#include "GJHDamageTextComponent.h"

#include "UI/Damage/GJHDamageTextWidget.h"

UGJHDamageTextComponent::UGJHDamageTextComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGJHDamageTextComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGJHDamageTextComponent::SetDamage(float InDamage)
{
	if (UGJHDamageTextWidget* DamageTextWidget = Cast<UGJHDamageTextWidget>(GetUserWidgetObject()))
	{
		DamageTextWidget->SetDamage(InDamage);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this] { DestroyComponent(); }, 2.f, false);
}
