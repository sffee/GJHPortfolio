#include "GJHInventorySlotWidget.h"

#include "Components/Border.h"
#include "GJHInventoryGridWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/SizeBox.h"
#include "Item/Definition/GJHItemInstance.h"

void UGJHInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	Border_Slot->SetBrushColor(HoveredColor);
	OnMouseEnterInventorySlotWidget.ExecuteIfBound(this, SlotIndex);
}

void UGJHInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	Border_Slot->SetBrushColor(DefaultColor);
	OnMouseLeaveInventorySlotWidget.ExecuteIfBound(this, SlotIndex);
}

FReply UGJHInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bMouseButtonDown = true;
	}
	
	return FReply::Handled();
}

FReply UGJHInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (bMouseButtonDown || ParentGridWidget->IsDraggedPickupItem())
		{
			bMouseButtonDown = false;
			OnDropPickupInventoryItem.ExecuteIfBound(this);
		}
	}
	
	return FReply::Handled();
}

void UGJHInventorySlotWidget::SetData(const int32 InSlotIndex, UGJHInventoryGridWidget* InGridWidget)
{
	SlotIndex = InSlotIndex;
	ParentGridWidget = InGridWidget;

	Border_Slot->SetBrushColor(DefaultColor);
}

void UGJHInventorySlotWidget::ClearItem()
{
	ItemInstance = nullptr;
}

void UGJHInventorySlotWidget::SetSlotDefaultColor()
{
	Border_Slot->SetBrushColor(DefaultColor);
}

void UGJHInventorySlotWidget::UpdateDraggedSlotColor(const FGJHDraggedInventoryItemResult InDraggedInventoryItemResult) const
{
	if (InDraggedInventoryItemResult.OverlappedItemCount == 0)
	{
		Border_Slot->SetBrushColor(DragEmptyPlacementColor);
	}
	else if (InDraggedInventoryItemResult.OverlappedItemCount == 1)
	{
		Border_Slot->SetBrushColor(DragValidPlacementColor);
	}
	else
	{
		Border_Slot->SetBrushColor(DragInvalidPlacementColor);
	}
}

FVector2D UGJHInventorySlotWidget::GetSize() const
{
	return FVector2D(SizeBox_Root->GetWidthOverride(), SizeBox_Root->GetHeightOverride());
}
