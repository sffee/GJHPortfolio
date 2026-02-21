#include "GJHAbilityTask_SweepAttack.h"

#include "Data/DataAsset/GJHBakeAttackAnimSequenceDataAsset.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystem/GJHAttackDataSubSystem.h"

static TAutoConsoleVariable<bool> CVarUseBakeAttackData(
	TEXT("GJH.UseBakeAttackData"),
	true,
	TEXT(""));

UGJHAbilityTask_SweepAttack::UGJHAbilityTask_SweepAttack()
{
	bTickingTask = true;
}

UGJHAbilityTask_SweepAttack* UGJHAbilityTask_SweepAttack::SweepAttack(
	UGameplayAbility* OwningAbility,
	UAnimMontage* MontageToPlay,
	FName SectionName,
	float StartMontagePosition,
	float TotalDuration,
	float TraceRadius,
	float TraceInterval,
	FName TraceStartSocketName,
	FName TraceEndSocketName,
	float DrawDebugTime)
{
	UGJHAbilityTask_SweepAttack* SweepAttack = NewAbilityTask<UGJHAbilityTask_SweepAttack>(OwningAbility);
	SweepAttack->MontageToPlay = MontageToPlay;
	SweepAttack->SectionName = SectionName;
	SweepAttack->StartMontagePosition = StartMontagePosition;
	SweepAttack->TotalDuration = TotalDuration + KINDA_SMALL_NUMBER;
	SweepAttack->TraceRadius = TraceRadius;
	SweepAttack->TraceInterval = TraceInterval;
	SweepAttack->TraceStartSocketName = TraceStartSocketName;
	SweepAttack->TraceEndSocketName = TraceEndSocketName;
	SweepAttack->DrawDebugTime = DrawDebugTime;
	
	return SweepAttack;
}

void UGJHAbilityTask_SweepAttack::Activate()
{
	if (IsValid(Ability) == false)
	{
		EndTask();
		return;
	}
	
	OwnerSkeletalMeshComponent = Ability->GetOwningComponentFromActorInfo();
	if (IsValid(OwnerSkeletalMeshComponent) == false)
	{
		EndTask();
		return;
	}
	
	if (IsValid(MontageToPlay) == false)
	{
		EndTask();
		return;
	}
	
	AttackDataAsset = UGJHAttackDataSubSystem::Get(this)->GetAttackData(MontageToPlay->GetName());
	if (IsValid(AttackDataAsset) == false)
	{
		EndTask();
		return;
	}
	
	StartWorldTime = GetWorld()->GetTimeSeconds();
	TickTask(0.f);
}

void UGJHAbilityTask_SweepAttack::TickTask(float DeltaTime)
{
#if CPUPROFILERTRACE_ENABLED
	TRACE_CPUPROFILER_EVENT_SCOPE_STR("SweepAttack");
#endif  // CPUPROFILERTRACE_ENABLED
	
	if (CVarUseBakeAttackData.GetValueOnAnyThread())
	{
		ProcessBakeData();
	}
	else
	{
		Process();
	}
}

void UGJHAbilityTask_SweepAttack::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	
	TickTask(0.f);
}

void UGJHAbilityTask_SweepAttack::Process()
{
	TArray<FHitResult> HitResults;
	TraceSocket(HitResults);
		
	if (HitResults.Num())
		OnTraceHit.ExecuteIfBound(HitResults);
}

void UGJHAbilityTask_SweepAttack::ProcessBakeData()
{
	const TArray<FGJHBakeAnimSocketData>& AttackData = AttackDataAsset->GetAttackData();
	
	const float CurrentWorldTime = GetWorld()->GetTimeSeconds();
	float ElapsedTime = CurrentWorldTime - StartWorldTime;
	ElapsedTime = FMath::Min(ElapsedTime, TotalDuration);
	
	while (ProcessedTime < ElapsedTime + KINDA_SMALL_NUMBER)
	{
		const float MontageTime = StartMontagePosition + ProcessedTime;
        
		while (DataIndex < AttackData.Num() - 1 && AttackData[DataIndex + 1].Time <= MontageTime)
			DataIndex++;

		const FGJHBakeAnimSocketData& BakeData = AttackData[DataIndex];
        
		if (BakeData.Time <= MontageTime)
		{
			TArray<FHitResult> HitResults;
			TraceSocket(BakeData, HitResults);
            
			if (HitResults.Num())
				OnTraceHit.ExecuteIfBound(HitResults);
		}

		ProcessedTime += TraceInterval;
	}
}

void UGJHAbilityTask_SweepAttack::TraceSocket(const FGJHBakeAnimSocketData& InAttackData, TArray<FHitResult>& OutHitResult)
{
	const FTransform ComponentTransform = OwnerSkeletalMeshComponent->GetComponentTransform();
	
	const FVector SocketStartLocation = ComponentTransform.TransformPosition(InAttackData.SocketStartLocation);
	const FVector SocketEndLocation = ComponentTransform.TransformPosition(InAttackData.SocketEndLocation);
	
	TraceSocket(SocketStartLocation, SocketEndLocation, OutHitResult);
	
	LastStartSocketLocation = InAttackData.SocketStartLocation;
	LastEndSocketLocation = InAttackData.SocketEndLocation;
}

void UGJHAbilityTask_SweepAttack::TraceSocket(TArray<FHitResult>& OutHitResult)
{
	const FVector SocketStartLocation = OwnerSkeletalMeshComponent->GetSocketLocation(TraceStartSocketName);
	const FVector SocketEndLocation = OwnerSkeletalMeshComponent->GetSocketLocation(TraceEndSocketName);
	
	TraceSocket(SocketStartLocation, SocketEndLocation, OutHitResult);
	
	LastStartSocketLocation = SocketStartLocation;
	LastEndSocketLocation = SocketEndLocation;
}

void UGJHAbilityTask_SweepAttack::TraceSocket(const FVector InStartSocketLocation, const FVector InEndSocketLocation, TArray<FHitResult>& OutHitResult)
{
	const FTransform ComponentTransform = OwnerSkeletalMeshComponent->GetComponentTransform();
	
	Trace(InStartSocketLocation, InEndSocketLocation, OutHitResult);
	
	if (LastStartSocketLocation.IsNearlyZero() == false)
	{
		const FVector LastStartLocation = ComponentTransform.TransformPosition(LastStartSocketLocation);
		
		Trace(LastStartLocation, InStartSocketLocation, OutHitResult);
		Trace(LastStartLocation, InEndSocketLocation, OutHitResult);
	}
	
	if (LastEndSocketLocation.IsNearlyZero() == false)
	{
		const FVector LastEndLocation = ComponentTransform.TransformPosition(LastEndSocketLocation);
		
		Trace(LastEndLocation, InStartSocketLocation, OutHitResult);
		Trace(LastEndLocation, InEndSocketLocation, OutHitResult);
	}
}

void UGJHAbilityTask_SweepAttack::Trace(const FVector& InTraceStart, const FVector& InTraceEnd, TArray<FHitResult>& OutHitResult)
{
	const EDrawDebugTrace::Type DrawDebugTrace = 0.f < DrawDebugTime ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(OwnerSkeletalMeshComponent, InTraceStart, InTraceEnd, TraceRadius, { UEngineTypes::ConvertToObjectType(ECC_Pawn) },
													 false, TArray<AActor*>(), DrawDebugTrace, OutHitResult, true, FLinearColor::Red, FLinearColor::Green, DrawDebugTime);
}
