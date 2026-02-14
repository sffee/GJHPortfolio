// Fill out your copyright notice in the Description page of Project Settings.


#include "GJHAnimNotifyState_AttackTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GJHAnimNotify_SendGameplayEvent.h"
#include "GameplayTag/GJHGameplayTag.h"
#include "Kismet/KismetSystemLibrary.h"

void UGJHAnimNotifyState_AttackTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UGJHAnimNotifyState_AttackTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp->GetWorld()->IsPreviewWorld())
		return;

	if (SocketNames.IsEmpty())
		return;
	
	if (PrevSocketLocations.IsEmpty())
	{
		for (int32 i = 0; i < SocketNames.Num(); ++i)
			PrevSocketLocations.Add(MeshComp->GetSocketLocation(SocketNames[i]));
	}

	FGameplayEventData EventData;
	
	AActor* OwnerActor = MeshComp->GetOwner();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TSet<AActor*> HitActors;
	for (int i = 0; i < SocketNames.Num(); ++i)
	{
		for (int j = 0; j < SocketNames.Num(); ++j)
		{
			const FVector TraceStart = PrevSocketLocations[i];
			const FVector TraceEnd = MeshComp->GetSocketLocation(SocketNames[j]);
		
			TArray<FHitResult> HitResults;
			EDrawDebugTrace::Type DrawDebugTrace = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

			UKismetSystemLibrary::SphereTraceMultiForObjects(MeshComp, TraceStart, TraceEnd, SphereSweepRadius, 
				ObjectTypes, false, ActorsToIgnore, DrawDebugTrace, HitResults, true);
		
			for (const FHitResult& HitResult : HitResults)
			{
				if (HitActors.Contains(HitResult.GetActor()))
					continue;

				HitActors.Add(HitResult.GetActor());

				FGameplayAbilityTargetData_SingleTargetHit* TargetHit = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
				EventData.TargetData.Add(TargetHit);
			}
		}

		PrevSocketLocations[i] = MeshComp->GetSocketLocation(SocketNames[i]);
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), FGJHGameplayTag::Ability_Event_SendTargetData(), EventData);
}

void UGJHAnimNotifyState_AttackTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

FString UGJHAnimNotifyState_AttackTrace::GetNotifyName_Implementation() const
{
	return FString("AttackTrace");
}
