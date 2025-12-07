#include "GJHGA_BasicAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "AbilitySystem/AbilityTask/GJHAbilityTask_PlayMontageAndWaitForEvent.h"
#include "GameplayTag/GJHGameplayTag.h"

UGJHGA_BasicAttack::UGJHGA_BasicAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	SetAssetTags(FGameplayTagContainer(FGJHGameplayTag::Ability_Skill_Active_BasicAttack()));
	ActivationOwnedTags.AddTag(FGJHGameplayTag::Ability_Skill_Active_BasicAttack());
}

bool UGJHGA_BasicAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AnimMontageSectionNames.IsEmpty())
		return false;
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGJHGA_BasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CurrentComboIndex = -1;
	SetNextAnimMontageSection();
}

void UGJHGA_BasicAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (bInputAvailable == false)
		return;
	
	SetNextAnimMontageSection();
}

void UGJHGA_BasicAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (IsValid(PlayMontageAndWaitForEvent))
		PlayMontageAndWaitForEvent->EndTask();
}

void UGJHGA_BasicAttack::SetNextAnimMontageSection()
{	
	FName NextAnimMontageSectionName = GetNextAnimMontageSectionName();
	if (NextAnimMontageSectionName == NAME_None)
	{
		K2_EndAbility();
		return;
	}
		
	bInputAvailable = false;
	HitActors.Empty();

	if (IsValid(PlayMontageAndWaitForEvent))
	{
		PlayMontageAndWaitForEvent->EndTask();
		PlayMontageAndWaitForEvent = nullptr;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FGJHGameplayTag::Ability_Event_InputAvailable());
	TagContainer.AddTag(FGJHGameplayTag::Ability_Event_SendTargetData());

	PlayMontageAndWaitForEvent = UGJHAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, AnimMontage, TagContainer, 1.f, NextAnimMontageSectionName);
	if (IsValid(PlayMontageAndWaitForEvent))
	{
		PlayMontageAndWaitForEvent->OnBlendOut.AddDynamic(this, &UGJHGA_BasicAttack::OnEndMontage);
		PlayMontageAndWaitForEvent->OnCancelled.AddDynamic(this, &UGJHGA_BasicAttack::OnEndMontage);
		PlayMontageAndWaitForEvent->OnInterrupted.AddDynamic(this, &UGJHGA_BasicAttack::OnEndMontage);
		PlayMontageAndWaitForEvent->OnCompleted.AddDynamic(this, &UGJHGA_BasicAttack::OnEndMontage);
		PlayMontageAndWaitForEvent->EventReceived.AddDynamic(this, &UGJHGA_BasicAttack::OnReceiveGameplayEvent);
		
		PlayMontageAndWaitForEvent->ReadyForActivation();
	}
}

FName UGJHGA_BasicAttack::GetNextAnimMontageSectionName()
{
	CurrentComboIndex = ((CurrentComboIndex + 1) % AnimMontageSectionNames.Num());
	return AnimMontageSectionNames[CurrentComboIndex];
}

void UGJHGA_BasicAttack::ApplyDamageToTarget(const FGameplayEventData& InEventData)
{
	for (int32 i = 0; i < InEventData.TargetData.Num(); ++i)
	{
		const FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(InEventData.TargetData, i);
		AActor* HitActor = HitResult.GetActor();

		if (HitActors.Contains(HitActor))
			continue;

		HitActors.Add(HitActor);

		ApplyDamage(GetDamage(CurrentComboIndex + 1), HitActor, CurrentComboIndex + 1);
	}
}

void UGJHGA_BasicAttack::OnEndMontage()
{
	K2_EndAbility();
}

void UGJHGA_BasicAttack::OnReceiveGameplayEvent(FGameplayEventData EventData)
{
	if (EventData.EventTag.MatchesTag(FGJHGameplayTag::Ability_Event_InputAvailable()))
	{
		bInputAvailable = true;
	}
	else if (EventData.EventTag.MatchesTag(FGJHGameplayTag::Ability_Event_SendTargetData()))
	{
		ApplyDamageToTarget(EventData);
	}
}
