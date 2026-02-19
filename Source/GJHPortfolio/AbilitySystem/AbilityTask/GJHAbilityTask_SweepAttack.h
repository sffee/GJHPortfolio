#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Data/DataAsset/GJHBakeAttackAnimSequenceDataAsset.h"
#include "GJHAbilityTask_SweepAttack.generated.h"

DECLARE_DELEGATE_OneParam(FGJHOnTraceHit, const TArray<FHitResult> HitResults);

class UGJHBakeAttackAnimSequenceDataAsset;

UCLASS()
class GJHPORTFOLIO_API UGJHAbilityTask_SweepAttack : public UAbilityTask
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> OwnerSkeletalMeshComponent;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageToPlay;
	
	float StartMontagePosition = 0.f;
	float StartWorldTime = 0.f;
	FName SectionName;
	float TotalDuration = 0.f;
	float TraceRadius = 0.f;
	float TraceInterval = 0.f;
	FName TraceStartSocketName;
	FName TraceEndSocketName;
	float DrawDebugTime = 0.f;
	
	int32 DataIndex = 0;
	
	float ProcessedTime = 0.f;
	
private:
	UPROPERTY()
	UGJHBakeAttackAnimSequenceDataAsset* AttackDataAsset;
	
	FVector LastStartSocketLocation;
	FVector LastEndSocketLocation;
	
public:
	FGJHOnTraceHit OnTraceHit;
	
public:
	UGJHAbilityTask_SweepAttack();
	
	static UGJHAbilityTask_SweepAttack* SweepAttack(
		UGameplayAbility* OwningAbility,
		UAnimMontage* MontageToPlay,
		FName SectionName,
		float StartMontagePosition,
		float TotalDuration,
		float TraceRadius,
		float TraceInterval,
		FName TraceStartSocketName,
		FName TraceEndSocketName,
		float DrawDebugTime);
	
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
private:
	void Process();
	void ProcessBakeData();
	void TraceSocket(const FGJHBakeAnimSocketData& InAttackData, TArray<FHitResult>& OutHitResult);
	void TraceSocket(TArray<FHitResult>& OutHitResult);
	void TraceSocket(const FVector InStartSocketLocation, const FVector InEndSocketLocation, TArray<FHitResult>& OutHitResult);
	void Trace(const FVector& InTraceStart, const FVector& InTraceEnd, TArray<FHitResult>& OutHitResult);
};
