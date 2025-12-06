#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GJHAnimNotifyState_AttackTrace.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHAnimNotifyState_AttackTrace : public UAnimNotifyState
{
	GENERATED_BODY()

private:	
	UPROPERTY(EditAnywhere, Category = "Trace")
	float SphereSweepRadius = 5.f;

	UPROPERTY(EditAnywhere, Category = "Trace")
	TArray<FName> SocketNames;

	UPROPERTY(EditAnywhere, Category = "Trace")
	bool bDrawDebug = false;

private:
	TArray<FVector> PrevSocketLocations;
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
};
