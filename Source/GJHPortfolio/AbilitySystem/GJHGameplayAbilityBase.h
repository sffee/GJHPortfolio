#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Character/GJHCharacterTypes.h"

#include "GJHGameplayAbilityBase.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Default", meta = (Categories = "Input"))
	FGameplayTag DefaultQuickSlotInputTag = FGameplayTag::EmptyTag;

protected:
	EGJHCharacterType CharacterType = EGJHCharacterType::None;

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	virtual FString GetDescription();
	float GetManaCost();
	float GetCooldown();
	
	FORCEINLINE FGameplayTag GetDefaultQuickSlotInputTag() const { return DefaultQuickSlotInputTag; }
};
