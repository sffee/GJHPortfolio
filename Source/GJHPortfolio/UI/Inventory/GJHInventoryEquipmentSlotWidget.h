#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventoryEquipmentSlotWidget.generated.h"

class UBorder;
class UImage;

UCLASS()
class GJHPORTFOLIO_API UGJHInventoryEquipmentSlotWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Slot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Slot;
	
private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	TObjectPtr<UTexture2D> SlotImage;

public:
	virtual void NativePreConstruct() override;
};
