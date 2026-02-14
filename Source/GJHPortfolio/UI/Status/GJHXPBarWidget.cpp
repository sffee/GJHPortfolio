#include "GJHXPBarWidget.h"

#include "Character/Player/GJHPlayerState.h"
#include "Components/ProgressBar.h"
#include "Library/GJHGameplayStatics.h"

void UGJHXPBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (AGJHPlayerState* GJHPlayerState = UGJHGameplayStatics::GetGJHPlayerState(this); IsValid(GJHPlayerState))
		GJHPlayerState->OnXPPercentChanged.AddUObject(this, &ThisClass::OnXPPercentChanged);
}

void UGJHXPBarWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (AGJHPlayerState* GJHPlayerState = UGJHGameplayStatics::GetGJHPlayerState(this); IsValid(GJHPlayerState))
		GJHPlayerState->OnXPPercentChanged.RemoveAll(this);
}

void UGJHXPBarWidget::OnXPPercentChanged(float NewPercent)
{
	if (ProgressBar_XP)
		ProgressBar_XP->SetPercent(NewPercent);
}
