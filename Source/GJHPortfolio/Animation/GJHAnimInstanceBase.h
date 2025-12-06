#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimNotify/GJHAnimNotify_SendGameplayEvent.h"
#include "GJHAnimInstanceBase.generated.h"

class UGJHAbilitySystemComponent;
class UCharacterMovementComponent;
class AGJHCharacterBase;

UCLASS()
class GJHPORTFOLIO_API UGJHAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bHasAcceleration = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bPlayedHitReaction = false;

private:
	FDelegateHandle HitReactionTagHandle;
	
private:
	TWeakObjectPtr<AGJHCharacterBase> OwningCharacter;
	TWeakObjectPtr<UCharacterMovementComponent> OwningCharacterMovementComponent;
	TWeakObjectPtr<UGJHAbilitySystemComponent> OwningAbilitySystemComponent;
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUninitializeAnimation() override;

private:
	void OnHitReactionTagUpdated(const FGameplayTag Tag, int32 NewCount);
};
