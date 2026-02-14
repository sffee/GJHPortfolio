#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "GJHAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FGJHAbilitiesGiven, const FGameplayAbilitySpec& AbilitySpec);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FGJHAbilityEquipped, int32 SkillIndex, FGameplayTag EquipQuickSlotInputTag, FGameplayTag PrevQuickSlotInputTag);
DECLARE_MULTICAST_DELEGATE_TwoParams(FGJHAbilityLevelChanged, int32 SkillIndex, int32 NewLevel);

UCLASS()
class GJHPORTFOLIO_API UGJHAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FGJHAbilitiesGiven OnAbilityGiven;
	FGJHAbilityEquipped OnAbilityEquipped;
	FGJHAbilityLevelChanged OnAbilityLevelChanged;
	
public:
	UGJHAbilitySystemComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AbilityInputPressed(const FGameplayTag& InInputTag);
	void AbilityInputPressedByAbilityTag(const FGameplayTag& InAbilityTag);
	void AbilityInputReleased(const FGameplayTag& InInputTag);
	void AbilityInputHeld(const FGameplayTag& InInputTag);

public:
	void AddAbility(const TSubclassOf<UGameplayAbility> InAbility);
	void AddAbilities(const TArray<TSubclassOf<UGameplayAbility>> InAbilities);

	void ApplyGameplayEffect(const TSubclassOf<UGameplayEffect> InGameplayEffect, int32 Level = -1);
	void ApplyGameplayEffects(const TArray<TSubclassOf<UGameplayEffect>> InGameplayEffects, int32 Level = -1);

public:
	void AddSkillLevel(const int32 InSkillIndex, int32 InAddPoint);

	int32 GetAbilityLevelBySkillIndex(const int32 InSkillIndex);
	FString GetDescriptionBySkillIndex(const int32 InSkillIndex);

public:
	void EquipAbility(const int32 InSkillIndex, const FGameplayTag& InQuickSlotInputTag);
	void CancelAbilityByAbilityTag(const FGameplayTag& InAbilityTag);
	void CancelAbilityByInputTag(const FGameplayTag& InInputTag);

private:
	bool AbilityHasAnyQuickSlot(FGameplayAbilitySpec* InAbilitySpec) const;
	void ClearQuickSlot(FGameplayAbilitySpec* InAbilitySpec);
	
public:
	FGameplayAbilitySpec* GetSpecByAbilityTag(const FGameplayTag& InAbilityTag);
	FGameplayAbilitySpec* GetSpecByQuickSlotInputTag(const FGameplayTag& InQuickSlotInputTag);
	FGameplayAbilitySpec* GetSpecBySkillIndex(const int32 InSkillIndex);
	FGameplayTag GetQuickSlotInputTagFromSpec(const FGameplayAbilitySpec* InAbilitySpec);
};
