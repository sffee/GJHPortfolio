#include "GJHAnimInstanceBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "Character/GJHCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTag/GJHGameplayTag.h"

void UGJHAnimInstanceBase::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwningCharacter = Cast<AGJHCharacterBase>(TryGetPawnOwner());
	OwningCharacterMovementComponent = OwningCharacter.IsValid() ? OwningCharacter->GetCharacterMovement() : nullptr;
	OwningAbilitySystemComponent = Cast<UGJHAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningCharacter.Get()));

	if (OwningAbilitySystemComponent.IsValid())
	{
		HitReactionTagHandle = OwningAbilitySystemComponent->RegisterGameplayTagEvent(FGJHGameplayTag::Ability_Common_HitReaction()).AddUObject(this, &ThisClass::OnHitReactionTagUpdated);
	}
}

void UGJHAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (OwningCharacter.IsValid() == false || OwningCharacterMovementComponent.IsValid() == false)
		return;
	
	const FVector Velocity = OwningCharacter->GetVelocity();
	Speed = Velocity.Size();
	bIsMoving = 0.f < Speed;
	bHasAcceleration = 0.f < OwningCharacterMovementComponent->GetCurrentAcceleration().Size();
}

void UGJHAnimInstanceBase::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();

	if (OwningAbilitySystemComponent.IsValid())
	{
		OwningAbilitySystemComponent->RegisterGameplayTagEvent(FGJHGameplayTag::Ability_Common_HitReaction()).Remove(HitReactionTagHandle);
	}
}

void UGJHAnimInstanceBase::OnHitReactionTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	bPlayedHitReaction = 0 < NewCount;
}
