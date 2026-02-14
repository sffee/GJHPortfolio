#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHStatusWidget.generated.h"

class UGJHAbilitySystemComponent;
class UProgressBar;

UCLASS(Abstract)
class GJHPORTFOLIO_API UGJHStatusWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Mana;

private:
	TWeakObjectPtr<UGJHAbilitySystemComponent> OwningAbilitySystemComponent;

	float CurrentHealth;
	float MaxHealth;
	float CurrentMana;
	float MaxMana;

	FDelegateHandle HealthAttributeDelegateHandles;
	FDelegateHandle MaxHealthAttributeDelegateHandles;
	FDelegateHandle ManaAttributeDelegateHandles;
	FDelegateHandle MaxManaAttributeDelegateHandles;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	void UpdateHealth() const;
	void UpdateMana() const;

private:
	void OnHealthAttributeChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxHealthAttributeChanged(const FOnAttributeChangeData& ChangeData);
	void OnManaAttributeChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxManaAttributeChanged(const FOnAttributeChangeData& ChangeData);
};
