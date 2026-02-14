#include "GJHGA_Death.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/GJHAbilityTypes.h"
#include "Character/GJHCharacterBase.h"
#include "GameplayTag/GJHGameplayTag.h"

UGJHGA_Death::UGJHGA_Death()
{
	SetAssetTags(FGameplayTagContainer(FGJHGameplayTag::Ability_Common_Death()));
	ActivationOwnedTags.AddTag(FGJHGameplayTag::Ability_Common_Death());
			
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGJHGameplayTag::Ability_Common_Death();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::Type::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

bool UGJHGA_Death::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (IsValid(DeathMontage) == false)
		return false;
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGJHGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const UGJHDeathInfo* DeathInfo = Cast<UGJHDeathInfo>(TriggerEventData->OptionalObject);
	if (IsValid(DeathInfo) && DeathInfo->DeathImpulse.IsNearlyZero() == false)
	{
		if (AGJHCharacterBase* GJHCharacter = Cast<AGJHCharacterBase>(ActorInfo->AvatarActor); IsValid(GJHCharacter))
			GJHCharacter->LaunchCharacter(DeathInfo->DeathImpulse, true, true);
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DeathMontage);
	if (IsValid(PlayMontageAndWait))
	{
		PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UGJHGA_Death::OnEndMontage);
		PlayMontageAndWait->OnCancelled.AddDynamic(this, &UGJHGA_Death::OnEndMontage);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UGJHGA_Death::OnEndMontage);
		PlayMontageAndWait->OnCompleted.AddDynamic(this, &UGJHGA_Death::OnEndMontage);
		
		PlayMontageAndWait->ReadyForActivation();
	}
}

void UGJHGA_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGJHGA_Death::OnEndMontage()
{
	K2_EndAbility();
}
