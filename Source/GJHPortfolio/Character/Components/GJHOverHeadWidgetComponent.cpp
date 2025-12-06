#include "GJHOverHeadWidgetComponent.h"

#include "Character/GJHCharacterBase.h"
#include "Subsystem/GJHUICosmeticSubsystem.h"

UGJHOverHeadWidgetComponent::UGJHOverHeadWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGJHOverHeadWidgetComponent::OnRegister()
{
	Super::OnRegister();

	if (UGJHUICosmeticSubsystem* UICosmeticSubSystem = UGJHUICosmeticSubsystem::Get(this); IsValid(UICosmeticSubSystem))
		UICosmeticSubSystem->RegisterOverHeadWidgetComponent(this);
}

void UGJHOverHeadWidgetComponent::OnUnregister()
{
	Super::OnUnregister();

	if (UGJHUICosmeticSubsystem* UICosmeticSubSystem = UGJHUICosmeticSubsystem::Get(this); IsValid(UICosmeticSubSystem))
		UICosmeticSubSystem->UnregisterOverHeadWidgetComponent(this);
}

void UGJHOverHeadWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = GetOwner<AGJHCharacterBase>();
}

bool UGJHOverHeadWidgetComponent::IsOwnerCharacterDeath() const
{
	if (OwningCharacter.IsValid())
		return OwningCharacter->IsDead();

	return false;
}
