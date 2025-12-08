#pragma once

#include "CoreMinimal.h"
#include "GJHDamageGameplayAbilityBase.h"

#include "GJHGA_BasicAttack.generated.h"

class UGJHAbilityTask_PlayMontageAndWaitForEvent;

UCLASS()
class GJHPORTFOLIO_API UGJHGA_BasicAttack : public UGJHDamageGameplayAbilityBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Ability|BasicAttack", meta = (Categories = "Combo", ForceInlineRow))
	TArray<FName> AnimMontageSectionNames;

	UPROPERTY(EditAnywhere, Category = "Ability|BasicAttack")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

private:
	int32 CurrentComboIndex = 1;
	bool bInputAvailable = false;

	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors;

	UPROPERTY()
	TObjectPtr<UGJHAbilityTask_PlayMontageAndWaitForEvent> PlayMontageAndWaitForEvent;
	
public:
	UGJHGA_BasicAttack();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

public:
	virtual FString GetDescription() override;

private:
	void SetNextAnimMontageSection();
	FName GetNextAnimMontageSectionName();

private:
	void ApplyDamageToTarget(const FGameplayEventData& InEventData);
	
private:
	UFUNCTION()
	void OnEndMontage();

	UFUNCTION()
	void OnReceiveGameplayEvent(FGameplayEventData EventData);
};
