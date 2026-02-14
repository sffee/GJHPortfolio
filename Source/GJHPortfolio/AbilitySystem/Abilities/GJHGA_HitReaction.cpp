#include "GJHGA_HitReaction.h"

#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/GJHAbilityTypes.h"
#include "AI/GJHAITypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/GJHCharacterBase.h"
#include "Character/Monster/GJHMonsterAIController.h"
#include "Character/Monster/GJHMonsterCharacter.h"
#include "GameplayTag/GJHGameplayTag.h"

UGJHGA_HitReaction::UGJHGA_HitReaction()
{
	SetAssetTags(FGameplayTagContainer(FGJHGameplayTag::Ability_Common_HitReaction()));
	ActivationOwnedTags.AddTag(FGJHGameplayTag::Ability_Common_HitReaction());
			
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGJHGameplayTag::Ability_Common_HitReaction();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::Type::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

bool UGJHGA_HitReaction::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (IsValid(HitReactionMontage) == false)
		return false;
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGJHGA_HitReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const UGJHReactionInfo* ReactionInfo = Cast<UGJHReactionInfo>(TriggerEventData->OptionalObject);
	if (IsValid(ReactionInfo) && ReactionInfo->KnockBackImpulse.IsNearlyZero() == false)
	{
		if (AGJHCharacterBase* GJHCharacter = Cast<AGJHCharacterBase>(ActorInfo->AvatarActor); IsValid(GJHCharacter))
			GJHCharacter->LaunchCharacter(ReactionInfo->KnockBackImpulse, true, true);
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, HitReactionMontage);
	if (IsValid(PlayMontageAndWait))
	{
		PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UGJHGA_HitReaction::OnEndMontage);
		PlayMontageAndWait->OnCancelled.AddDynamic(this, &UGJHGA_HitReaction::OnEndMontage);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UGJHGA_HitReaction::OnEndMontage);
		PlayMontageAndWait->OnCompleted.AddDynamic(this, &UGJHGA_HitReaction::OnEndMontage);
		
		PlayMontageAndWait->ReadyForActivation();
	}
}

void UGJHGA_HitReaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGJHGA_HitReaction::OnEndMontage()
{
	K2_EndAbility();
}
