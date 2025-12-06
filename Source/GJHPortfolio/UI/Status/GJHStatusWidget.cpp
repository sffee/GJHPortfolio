#include "GJHStatusWidget.h"

#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Library/GJHAbilitySystemStatics.h"

void UGJHStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OwningAbilitySystemComponent = UGJHAbilitySystemStatics::GetAbilitySystemComponent(GetOwningPlayer());
	if (OwningAbilitySystemComponent.IsValid())
	{
		bool bFound = false;
		CurrentHealth = OwningAbilitySystemComponent->GetGameplayAttributeValue(UGJHCharacterAttributeSet::GetHealthAttribute(), bFound); 
		MaxHealth = OwningAbilitySystemComponent->GetGameplayAttributeValue(UGJHCharacterAttributeSet::GetMaxHealthAttribute(), bFound); 
		CurrentMana = OwningAbilitySystemComponent->GetGameplayAttributeValue(UGJHCharacterAttributeSet::GetManaAttribute(), bFound); 
		MaxMana = OwningAbilitySystemComponent->GetGameplayAttributeValue(UGJHCharacterAttributeSet::GetMaxManaAttribute(), bFound);

		UpdateHealth();
		UpdateMana();
		
		HealthAttributeDelegateHandles = OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
		MaxHealthAttributeDelegateHandles = OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
		ManaAttributeDelegateHandles = OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetManaAttribute()).AddUObject(this, &ThisClass::OnManaAttributeChanged);
		MaxManaAttributeDelegateHandles = OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetMaxManaAttribute()).AddUObject(this, &ThisClass::OnManaAttributeChanged);
	}
}

void UGJHStatusWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (OwningAbilitySystemComponent.IsValid())
	{
		OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetHealthAttribute()).Remove(HealthAttributeDelegateHandles);
		OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetMaxHealthAttribute()).Remove(MaxHealthAttributeDelegateHandles);
		OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetMaxHealthAttribute()).Remove(ManaAttributeDelegateHandles);
		OwningAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGJHCharacterAttributeSet::GetMaxHealthAttribute()).Remove(MaxManaAttributeDelegateHandles);
	}
}

void UGJHStatusWidget::UpdateHealth() const
{
	const float Percent = CurrentHealth / MaxHealth;
	ProgressBar_Health->SetPercent(Percent);
}

void UGJHStatusWidget::UpdateMana() const
{
	const float Percent = CurrentMana / MaxMana;
	ProgressBar_Mana->SetPercent(Percent);
}

void UGJHStatusWidget::OnHealthAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHealth();
}

void UGJHStatusWidget::OnMaxHealthAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	MaxHealth = ChangeData.NewValue;
	UpdateHealth();
}

void UGJHStatusWidget::OnManaAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMana = ChangeData.NewValue;
	UpdateMana();
}

void UGJHStatusWidget::OnMaxManaAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	MaxMana = ChangeData.NewValue;
	UpdateMana();
}
