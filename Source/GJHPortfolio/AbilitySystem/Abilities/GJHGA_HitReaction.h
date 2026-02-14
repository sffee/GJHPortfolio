#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GJHGameplayAbilityBase.h"
#include "GJHGA_HitReaction.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHGA_HitReaction : public UGJHGameplayAbilityBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Ability|HitReaction")
	TObjectPtr<UAnimMontage> HitReactionMontage;
	
public:
	UGJHGA_HitReaction();
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnEndMontage();
};
