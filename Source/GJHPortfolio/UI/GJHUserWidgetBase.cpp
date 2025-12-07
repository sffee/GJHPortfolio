#include "GJHUserWidgetBase.h"

#include "Library/GJHAbilitySystemStatics.h"
#include "Library/GJHGameplayStatics.h"

void UGJHUserWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGJHUserWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UGJHUserWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGJHUserWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGJHUserWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

UGJHAbilitySystemComponent* UGJHUserWidgetBase::GetAbilitySystemComponent() const
{
	return UGJHAbilitySystemStatics::GetAbilitySystemComponent(GetOwningPlayer());
}

AGJHPlayerState* UGJHUserWidgetBase::GetPlayerState() const
{
	return UGJHGameplayStatics::GetGJHPlayerState(GetOwningPlayer());
}
