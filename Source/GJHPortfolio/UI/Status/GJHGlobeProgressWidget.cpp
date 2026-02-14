#include "GJHGlobeProgressWidget.h"

#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

UGJHGlobeProgressWidget::UGJHGlobeProgressWidget()
{
}

void UGJHGlobeProgressWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(ProgressBar_Globe) && IsValid(GlobeMaterial))
	{
		FProgressBarStyle ProgressBarStyle = ProgressBar_Globe->GetWidgetStyle();
		FSlateBrush SlateBrush = ProgressBarStyle.FillImage;
		SlateBrush.SetResourceObject(GlobeMaterial);
		ProgressBarStyle.SetFillImage(SlateBrush);
		ProgressBar_Globe->SetWidgetStyle(ProgressBarStyle);
	}

	if (IsValid(ProgressBar_GhostGlobe) && IsValid(GhostGlobeMaterial))
	{
		FProgressBarStyle ProgressBarStyle = ProgressBar_Globe->GetWidgetStyle();
		FSlateBrush SlateBrush = ProgressBarStyle.FillImage;
		SlateBrush.SetResourceObject(GhostGlobeMaterial);
		ProgressBarStyle.SetFillImage(SlateBrush);
		ProgressBar_GhostGlobe->SetWidgetStyle(ProgressBarStyle);
	}
}

void UGJHGlobeProgressWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGJHGlobeProgressWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentGhostGlobePercent = UKismetMathLibrary::FInterpTo(CurrentGhostGlobePercent, TargetGhostGlobePercent, InDeltaTime, 1.f);
	ProgressBar_GhostGlobe->SetPercent(CurrentGhostGlobePercent);
}

void UGJHGlobeProgressWidget::SetGlobePercent(float InPercent)
{
	if (IsValid(ProgressBar_Globe))
		ProgressBar_Globe->SetPercent(InPercent);

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this](float GlobePercent)
	{
		TargetGhostGlobePercent = GlobePercent;
	}, InPercent);
	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, TimerDelegate, 0.5f, false);
}
