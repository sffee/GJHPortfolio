#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventoryItemWidget.generated.h"

class UBorder;
class UTextBlock;
class UImage;
class UGJHItemInstance;

UCLASS()
class GJHPORTFOLIO_API UGJHInventoryItemWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Quantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Slot;

private:
	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DefaultColor = FLinearColor(0.f, 0.f, 0.f, 0.f);
	
	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor HoveredColor = FLinearColor(0.87f, 0.56f, 0.063f, 0.75f);

	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DragValidPlacementColor = FLinearColor(0.43f, 0.87f, 0.036f, 0.75f);

	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DragInvalidPlacementColor = FLinearColor(1.f, 0.03f, 0.04f, 0.75f);
	
private:
	TWeakObjectPtr<UGJHItemInstance> ItemInstance;

public:
	void SetItemInstance(UGJHItemInstance* InItemInstance);
	
public:
	UGJHItemInstance* GetItemInstance() const { return ItemInstance.Get(); }
};
