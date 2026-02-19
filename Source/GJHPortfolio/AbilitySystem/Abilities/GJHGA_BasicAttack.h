#pragma once

#include "CoreMinimal.h"
#include "GJHDamageGameplayAbilityBase.h"

#include "GJHGA_BasicAttack.generated.h"

class UGJHAbilityTask_SweepAttack;
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
	
	UPROPERTY(EditAnywhere, Category = "Ability|BasicAttack")
	float TraceRadius = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Ability|BasicAttack")
	float TraceInterval = 0.1f;
	
	UPROPERTY(EditAnywhere, Category = "Ability|BasicAttack")
	FName TraceStartSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Ability|BasicAttack")
	FName TraceEndSocketName;
	
private:
	UPROPERTY(EditAnywhere, Category = "Ability|BasicAttack|Debug")
	bool bDrawDebug = false;
	
	UPROPERTY(EditAnywhere, Category = "Ability|BasicAttack|Debug")
	float DrawDebugTime = 1.f;

private:
	int32 CurrentComboIndex = 1;
	bool bInputAvailable = false;

	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors;

	UPROPERTY()
	TObjectPtr<UGJHAbilityTask_PlayMontageAndWaitForEvent> PlayMontageAndWaitForEvent;
	
	UPROPERTY()
	TObjectPtr<UGJHAbilityTask_SweepAttack> SweepAttack;
	
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
	FName GetCurrentAnimMontageSectionName();
	FName GetNextAnimMontageSectionName();

private:
	void ApplyDamageToTarget(const FGameplayEventData& InEventData);
	void ApplyDamageToTarget(const TArray<FHitResult>& InHitResults);
	void ApplyDamageToTarget(const FHitResult& InHitResult);
	void StartSweepAttack(float InTotalDuration);
	void EndSweepAttack();
	
private:
	UFUNCTION()
	void OnEndMontage();

	UFUNCTION()
	void OnReceiveGameplayEvent(FGameplayEventData EventData);
	
	void OnTraceHit(const TArray<FHitResult> HitResults);
};
