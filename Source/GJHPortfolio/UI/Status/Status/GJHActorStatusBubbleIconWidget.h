#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHActorStatusBubbleIconWidget.generated.h"

class UImage;
class UMaterialInterface;

UCLASS()
class GJHPORTFOLIO_API UGJHActorStatusBubbleIconWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_StatusIcon;
	
public:
	void SetMaterial(UMaterialInterface* InMaterial);
};
