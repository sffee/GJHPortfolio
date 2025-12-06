#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHOverHeadStatusWidget.generated.h"

class UGJHAbilitySystemComponent;
class UProgressBar;

UCLASS()
class GJHPORTFOLIO_API UGJHOverHeadStatusWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Health;

private:
	TWeakObjectPtr<UGJHAbilitySystemComponent> OwningAbilitySystemComponent;

	float CurrentHealth;
	float MaxHealth;

	FDelegateHandle HealthAttributeDelegateHandles;
	FDelegateHandle MaxHealthAttributeDelegateHandles;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void SetAbilitySystemComponent(UGJHAbilitySystemComponent* InAbilitySystemComponent);
	
private:
	void UpdateHealth() const;

private:
	void OnHealthAttributeChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxHealthAttributeChanged(const FOnAttributeChangeData& ChangeData);
};
