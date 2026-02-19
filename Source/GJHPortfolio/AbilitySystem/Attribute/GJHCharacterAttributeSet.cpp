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

void UGJHCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UGJHCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGJHCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGJHCharacterAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGJHCharacterAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
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
		}
		else
		{
			SendHitReactionEvent(InGJHEffectProperties);
		}

		CreateDamageText(Damage, InGJHEffectProperties);
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
			PlayerController->Client_CreateDamageText(InGJHEffectProperties.TargetAvatarActor, InDamage);
	}
}

void UGJHCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGJHCharacterAttributeSet, Health, OldHealth);
}

void UGJHCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGJHCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UGJHCharacterAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGJHCharacterAttributeSet, Mana, OldMana);
}

void UGJHCharacterAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGJHCharacterAttributeSet, MaxMana, OldMaxMana);
}
