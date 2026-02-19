#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GJHAnimNotifyState_SendGameplayEvent.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHAnimNotifyState_SendGameplayEvent : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Tag")
	FGameplayTag StartTag;
	
	UPROPERTY(EditAnywhere, Category = "Tag")
	FGameplayTag EndTag;
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	virtual FString GetNotifyName_Implementation() const override;
};
