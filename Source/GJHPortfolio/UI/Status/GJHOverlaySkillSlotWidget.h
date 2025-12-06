#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHOverlaySkillSlotWidget.generated.h"

class UAbilitySystemComponent;
class UBorder;
class UProgressBar;
class UTextBlock;
class UGJHSkillSlotWidget;

UCLASS()
class GJHPORTFOLIO_API UGJHOverlaySkillSlotWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGJHSkillSlotWidget> SkillSlotWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Cooldown;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> Progress_Cooldown;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Cooldown;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_DragOver;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Animation_EndCooldown;

private:
	UPROPERTY(EditAnywhere, Category = "GJH|Input", meta = (Categories = "Input.QuickSlot"))
	FGameplayTag QuickSlotInputTag;

	int32 SlotSkillIndex = -1;
	FGameplayTag CooldownTag;

	float ReminingCooldown;
	float Cooldown;
	FTimerHandle CooldownTimerHandle;

public:
	UGJHOverlaySkillSlotWidget();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void UpdateSlot(const int32 InSkillIndex, const FGameplayTag& InEquipQuickSlotInputTag, const FGameplayTag& PrevQuickSlotInputTag);
	void UpdateSlot(const FGameplayAbilitySpec* InAbilitySpec);

private:
	void ClearSlot();
	
private:
	void UpdateCooldown();
	void ProgressCooldown(const FGameplayTagContainer& InGameplayTagContainer);

	void OnAbilityGiven(const FGameplayAbilitySpec& AbilitySpec);
	void OnActiveEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle);

public:
	FORCEINLINE FGameplayTag GetQuickSlotInputTag() const { return QuickSlotInputTag; }
	
private:
	void OnAbilityEquipped(int32 SkillIndex, FGameplayTag EquipQuickSlotInputTag, FGameplayTag PrevQuickSlotInputTag);
};
