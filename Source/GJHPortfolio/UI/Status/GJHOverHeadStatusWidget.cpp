#include "GJHOverHeadStatusWidget.h"

#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Library/GJHAbilitySystemStatics.h"

void UGJHOverHeadStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGJHOverHeadStatusWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (OwningAbilitySystemComponent.IsValid())
	{
		OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetHealthAttribute()).Remove(HealthAttributeDelegateHandles);
		OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetMaxHealthAttribute()).Remove(MaxHealthAttributeDelegateHandles);
	}
}

void UGJHOverHeadStatusWidget::SetAbilitySystemComponent(UGJHAbilitySystemComponent* InAbilitySystemComponent)
{
	OwningAbilitySystemComponent = InAbilitySystemComponent;
	
	if (OwningAbilitySystemComponent.IsValid())
	{
		bool bFound = false;
		CurrentHealth = OwningAbilitySystemComponent->GetGameplayAttributeValue(UGJHCharacterAttributeSet::GetHealthAttribute(), bFound); 
		MaxHealth = OwningAbilitySystemComponent->GetGameplayAttributeValue(UGJHCharacterAttributeSet::GetMaxHealthAttribute(), bFound); 

		UpdateHealth();
		
		HealthAttributeDelegateHandles = OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
		MaxHealthAttributeDelegateHandles = OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
	}
}

void UGJHOverHeadStatusWidget::UpdateHealth() const
{
	const float Percent = CurrentHealth / MaxHealth;
	ProgressBar_Health->SetPercent(Percent);
}

void UGJHOverHeadStatusWidget::OnHealthAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHealth();
}

void UGJHOverHeadStatusWidget::OnMaxHealthAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	MaxHealth = ChangeData.NewValue;
	UpdateHealth();
}
