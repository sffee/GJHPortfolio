#include "GJHUIStatics.h"

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
