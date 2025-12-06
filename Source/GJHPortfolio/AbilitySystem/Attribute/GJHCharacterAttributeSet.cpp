#include "GJHCharacterAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/GJHAbilityTypes.h"
#include "Character/GJHCharacterBase.h"
#include "Character/Monster/GJHMonsterCharacter.h"
#include "Character/Player/GJHPlayerController.h"
#include "GameplayTag/GJHGameplayTag.h"
#include "Library/GJHDataStatics.h"
#include "Library/GJHGameplayStatics.h"
#include "Net/UnrealNetwork.h"

void UGJHCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	
	if (Attribute == GetManaAttribute())
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
}

void UGJHCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGJHEffectProperties GJHEffectProperties;
	SetEffectProperties(Data, GJHEffectProperties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
		HandleIncomingDamage(Data, GJHEffectProperties);
}

void UGJHCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UGJHCharacterAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& InData, FGJHEffectProperties& OutProperties)
{
	OutProperties.EffectContextHandle = InData.EffectSpec.GetContext();
	OutProperties.GJHGameplayEffectContext = static_cast<FGJHGameplayEffectContext*>(OutProperties.EffectContextHandle.Get());
	OutProperties.SourceASC = OutProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(OutProperties.SourceASC) && OutProperties.SourceASC->AbilityActorInfo.IsValid() && OutProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		OutProperties.SourceAvatarActor = OutProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();
		OutProperties.SourceController = OutProperties.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (IsValid(OutProperties.SourceController) == false && IsValid(OutProperties.SourceAvatarActor))
		{
			if (const APawn* Pawn = Cast<APawn>(OutProperties.SourceAvatarActor))
				OutProperties.SourceController = Pawn->GetController();
		}
		
		if (IsValid(OutProperties.SourceController))
			OutProperties.SourceCharacter = Cast<AGJHCharacterBase>(OutProperties.SourceController->GetPawn());
	}

	if (InData.Target.AbilityActorInfo.IsValid() && InData.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		OutProperties.TargetAvatarActor = InData.Target.AbilityActorInfo->AvatarActor.Get();
		OutProperties.TargetController = InData.Target.AbilityActorInfo->PlayerController.Get();
		OutProperties.TargetCharacter = Cast<AGJHCharacterBase>(OutProperties.TargetAvatarActor);
		OutProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutProperties.TargetAvatarActor);
	}
}

void UGJHCharacterAttributeSet::HandleIncomingDamage(const FGameplayEffectModCallbackData& Data, const FGJHEffectProperties& InGJHEffectProperties)
{
	const float Damage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	
	if (0.f < Damage)
	{
		const float NewHealth = GetHealth() - Damage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bDead = NewHealth <= 0.f;
		if (bDead)
		{
			SendDeathEvent(InGJHEffectProperties);

			ProcessAddXP(InGJHEffectProperties);
			// ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			// if (CombatInterface)
			// {
			// 	FVector Impulse = UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
			// 	CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
			// }
			// SendXPEvent(Props);
		}
		else
		{
			SendHitReactionEvent(InGJHEffectProperties);
			
			// if (Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetCharacter))
			// {
			// 	FGameplayTagContainer TagContainer;
			// 	TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
			// 	Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			// }
			//
			// const FVector& KnockbackForce = UAuraAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
			// if (!KnockbackForce.IsNearlyZero(1.f))
			// {
			// 	Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			// }
		}

		CreateDamageText(Damage, InGJHEffectProperties);
			
		// const bool bBlock = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		// const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		// ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
		// if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		// {
		// 	Debuff(Props);
		// }
	}
}

void UGJHCharacterAttributeSet::ProcessAddXP(const FGJHEffectProperties& InGJHEffectProperties) const
{
	AGJHMonsterCharacter* MonsterCharacter = Cast<AGJHMonsterCharacter>(InGJHEffectProperties.TargetCharacter);
	if (IsValid(MonsterCharacter) == false)
		return;
	
	const float RewardXP = UGJHDataStatics::GetMonsterXP(MonsterCharacter, MonsterCharacter->GetMonsterIndex(), MonsterCharacter->GetMonsterLevel());
	if (RewardXP <= 0.f)
		return;

	UGJHGameplayStatics::AddXP(InGJHEffectProperties.SourceCharacter, RewardXP);
}

void UGJHCharacterAttributeSet::SendHitReactionEvent(const FGJHEffectProperties& InGJHEffectProperties) const
{
	FGameplayEventData EventData;
			
	UGJHReactionInfo* ReactionInfo = NewObject<UGJHReactionInfo>();
	ReactionInfo->KnockBackImpulse = InGJHEffectProperties.GJHGameplayEffectContext->GetKnockBackImpulse();
	EventData.OptionalObject = ReactionInfo;
			
	InGJHEffectProperties.TargetASC->HandleGameplayEvent(FGJHGameplayTag::Ability_Common_HitReaction(), &EventData);
}

void UGJHCharacterAttributeSet::SendDeathEvent(const FGJHEffectProperties& InGJHEffectProperties) const
{
	FGameplayEventData EventData;
			
	UGJHDeathInfo* DeathInfo = NewObject<UGJHDeathInfo>();
	DeathInfo->DeathImpulse = InGJHEffectProperties.GJHGameplayEffectContext->GetKnockBackImpulse();
	EventData.OptionalObject = DeathInfo;
			
	InGJHEffectProperties.TargetASC->HandleGameplayEvent(FGJHGameplayTag::Ability_Common_Death(), &EventData);
}

void UGJHCharacterAttributeSet::CreateDamageText(float InDamage, const FGJHEffectProperties& InGJHEffectProperties)
{
	if (IsValid(InGJHEffectProperties.SourceCharacter) == false || IsValid(InGJHEffectProperties.TargetCharacter) == false)
		return;

	if (InGJHEffectProperties.SourceCharacter != InGJHEffectProperties.TargetCharacter)
	{
		if (AGJHPlayerController* PlayerController = Cast<AGJHPlayerController>(InGJHEffectProperties.SourceCharacter->Controller))
			PlayerController->CreateDamageText(InGJHEffectProperties.TargetAvatarActor, InDamage);
	}
}
