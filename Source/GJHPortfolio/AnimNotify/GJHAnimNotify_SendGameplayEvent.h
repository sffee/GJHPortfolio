#pragma once

#include "CoreMinimal.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "GJHAnimNotify_SendGameplayEvent.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag GameplayTag;

	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayEventData EventData;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};
