#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GJHSettings/GJHUIDeveloperSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystem/GJHUISubSystem.h"
#include "GJHUIStatics.generated.h"

class UWidget;
class UGJHUserWidgetBase;

UCLASS()
class GJHPORTFOLIO_API UGJHUIStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TSubclassOf<UGJHUserWidgetBase> GetUIWidgetClass(const FGameplayTag& InUITypeTag);
	static FGJHUISetting GetUIStateSetting(const FGameplayTag& InUITypeTag);

public:
	static FVector2D GetWidgetViewportPosition(UWidget* InWidget, const FVector2D& InOffset = FVector2D::ZeroVector);
};
