#include "GJHOverlayWidget.h"

#include "GJHGlobeProgressWidget.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "Library/GJHAbilitySystemStatics.h"

void UGJHOverlayWidget::NativeOnInitialized()
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

void UGJHOverlayWidget::NativeDestruct()
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

void UGJHOverlayWidget::UpdateHealth() const
{
	const float Percent = CurrentHealth / MaxHealth;
	HealthGlobe->SetGlobePercent(Percent);
}

void UGJHOverlayWidget::UpdateMana() const
{
	const float Percent = CurrentMana / MaxMana;
	ManaGlobe->SetGlobePercent(Percent);
}

void UGJHOverlayWidget::OnHealthAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHealth();
}

void UGJHOverlayWidget::OnMaxHealthAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	MaxHealth = ChangeData.NewValue;
	UpdateHealth();
}

void UGJHOverlayWidget::OnManaAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMana = ChangeData.NewValue;
	UpdateMana();
}

void UGJHOverlayWidget::OnMaxManaAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	MaxMana = ChangeData.NewValue;
	UpdateMana();
}
