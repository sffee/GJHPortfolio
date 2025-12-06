#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHOverlayWidget.generated.h"

class UGJHAbilitySystemComponent;
class UGJHGlobeProgressWidget;

UCLASS()
class GJHPORTFOLIO_API UGJHOverlayWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGJHGlobeProgressWidget> HealthGlobe;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGJHGlobeProgressWidget> ManaGlobe;

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
