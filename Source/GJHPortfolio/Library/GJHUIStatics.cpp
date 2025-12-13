#include "GJHUIStatics.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHSettings/GJHUIDeveloperSettings.h"

TSubclassOf<UGJHUserWidgetBase> UGJHUIStatics::GetUIWidgetClass(const FGameplayTag& InUITypeTag)
{
	const UGJHUIDeveloperSettings* GJHUIDeveloperSettings = GetDefault<UGJHUIDeveloperSettings>();
	if (GJHUIDeveloperSettings->WidgetMap.Contains(InUITypeTag) == false)
	{
		ensureMsgf(false, TEXT("Widget Not Found!!"));
		return nullptr;
	}

	return GJHUIDeveloperSettings->WidgetMap[InUITypeTag].WidgetClass.LoadSynchronous();
}

FGJHUISetting UGJHUIStatics::GetUIStateSetting(const FGameplayTag& InUITypeTag)
{
	const UGJHUIDeveloperSettings* GJHUIDeveloperSettings = GetDefault<UGJHUIDeveloperSettings>();
	if (GJHUIDeveloperSettings->WidgetMap.Contains(InUITypeTag) == false)
	{
		ensureMsgf(false, TEXT("Widget Not Found!!"));
		return FGJHUISetting();
	}

	return GJHUIDeveloperSettings->WidgetMap[InUITypeTag];
}

FVector2D UGJHUIStatics::GetWidgetViewportPosition(UWidget* InWidget, const FVector2D& InOffset)
{
	const FGeometry Geometry = InWidget->GetCachedGeometry();

	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(InWidget, InWidget->GetCachedGeometry(), InOffset, PixelPosition, ViewportPosition);
	
	return ViewportPosition;
}
