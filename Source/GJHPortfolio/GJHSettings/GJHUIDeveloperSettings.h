#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "GJHUIDeveloperSettings.generated.h"

class UGJHUserWidgetBase;

USTRUCT()
struct FGJHUISetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UGJHUserWidgetBase> WidgetClass;

	UPROPERTY(EditAnywhere)
	bool bShowMouseCursor = false;
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "GJH UI Settings"))
class GJHPORTFOLIO_API UGJHUIDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget", meta = (ForceInlineRow, Categories = "UI.Type"))
	TMap<FGameplayTag, FGJHUISetting> WidgetMap;

	UPROPERTY(Config, EditAnywhere, Category = "Cosmetics")
	float OverHeadWidgetThreshold = 1000.f;
};
