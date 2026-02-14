#pragma once

#include "CoreMinimal.h"
#include "GJHInventorySubWidgetBase.h"
#include "UI/GJHUITypes.h"
#include "GJHInventorySlotWidget.generated.h"

class USizeBox;
class UGJHItemInstance;
class UGJHInventoryGridWidget;
class UBorder;

DECLARE_DELEGATE_TwoParams(FGJHOnMouseEventInventorySlotWidget, class UGJHInventorySlotWidget* InventorySlotWidget, int32 SlotIndex);
DECLARE_DELEGATE_OneParam(FGJHOnDropPickupItem, class UGJHInventorySlotWidget* InventorySlotWidget);

UCLASS()
class GJHPORTFOLIO_API UGJHInventorySlotWidget : public UGJHInventorySubWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Slot;
	
private:
	TWeakObjectPtr<UGJHInventoryGridWidget> ParentGridWidget;

	bool bMouseButtonDown = false;

public:
	FGJHOnMouseEventInventorySlotWidget OnMouseEnterInventorySlotWidget; 
	FGJHOnMouseEventInventorySlotWidget OnMouseLeaveInventorySlotWidget; 
	FGJHOnDropPickupItem OnDropPickupInventoryItem;

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetData(const int32 InSlotIndex, UGJHInventoryGridWidget* InGridWidget);
	void ClearItem();
	
public:
	void SetSlotDefaultColor();
	void UpdateDraggedSlotColor(const FGJHDraggedInventoryItemResult InDraggedInventoryItemResult) const;
	
public:
	FVector2D GetSize() const;
};
