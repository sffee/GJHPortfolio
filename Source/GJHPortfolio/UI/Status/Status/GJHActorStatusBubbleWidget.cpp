#include "GJHActorStatusBubbleWidget.h"

#include "GJHActorStatusBubbleIconWidget.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "GJHSettings/GJHUIDeveloperSettings.h"
#include "Library/GJHDataStatics.h"
#include "Library/GJHUIStatics.h"

void UGJHActorStatusBubbleWidget::InitStatus(const FGameplayTag& InStatusTag, int32 InCount)
{
	StatusTag = LastStatusTag = InStatusTag;
	MID = UMaterialInstanceDynamic::Create(BubbleMaterial, GetWorld());
	
	RefreshStatus(StatusTag, InCount);
}

void UGJHActorStatusBubbleWidget::RefreshStatus(const FGameplayTag& InStatusTag, int32 InCount)
{
	if (InStatusTag.MatchesTag(LastStatusTag) == false)
		return;
	
	LastStatusTag = InStatusTag;
	
	WrapBox_Bubble->ClearChildren();
	
	FGJHStatusData StatusData = UGJHDataStatics::GetStatusData(InStatusTag);
	UTexture2D* StatusIcon = StatusData.Icon.LoadSynchronous();
	if (IsValid(StatusIcon) == false)
		return;
	
	MID->SetTextureParameterValue(FName("Image"), StatusIcon);
	
	for (int32 i = 0; i < InCount; ++i)
	{
		UGJHActorStatusBubbleIconWidget* StatusIconWidget = CreateWidget<UGJHActorStatusBubbleIconWidget>(GetWorld(), BubbleIconWidgetClass);
		StatusIconWidget->SetMaterial(MID);
		
		WrapBox_Bubble->AddChildToWrapBox(StatusIconWidget);
	}
}
