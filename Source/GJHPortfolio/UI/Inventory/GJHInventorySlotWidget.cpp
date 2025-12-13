#include "GJHInventorySlotWidget.h"

#include "GJHDraggedInventoryItemWidget.h"
#include "Components/Border.h"
#include "GJHInventoryGridWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/SizeBox.h"
#include "Item/Definition/GJHItemInstance.h"

void UGJHInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	Border_Slot->SetBrushColor(HoveredColor);
}

void UGJHInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	Border_Slot->SetBrushColor(DefaultColor);
}

FReply UGJHInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UGJHInventorySlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UGJHDraggedInventoryItemWidget* DraggedItemWidget = Cast<UGJHDraggedInventoryItemWidget>(InOperation->DefaultDragVisual);
	if (IsValid(DraggedItemWidget) == false)
		return;
	
	OnDragEnterInventoryItemWidget.ExecuteIfBound(DraggedItemWidget, SlotIndex);
}

void UGJHInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UGJHDraggedInventoryItemWidget* DraggedItemWidget = Cast<UGJHDraggedInventoryItemWidget>(InOperation->DefaultDragVisual);
	if (IsValid(DraggedItemWidget) == false)
		return;
	
	OnDragLeaveInventoryItemWidget.ExecuteIfBound(DraggedItemWidget, SlotIndex);
}

void UGJHInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UGJHDraggedInventoryItemWidget* DraggedItemWidget = Cast<UGJHDraggedInventoryItemWidget>(InOperation->DefaultDragVisual);
	if (IsValid(DraggedItemWidget) == false)
		return;
	
	OnDragCancelledInventoryItemWidget.ExecuteIfBound(DraggedItemWidget, SlotIndex);
}

bool UGJHInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UGJHDraggedInventoryItemWidget* DraggedItemWidget = Cast<UGJHDraggedInventoryItemWidget>(InOperation->DefaultDragVisual);
	if (IsValid(DraggedItemWidget) == false)
		return false;
	
	OnDropInventoryItemWidget.ExecuteIfBound(DraggedItemWidget, SlotIndex);
	return true;
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

bool UGJHInventorySlotWidget::IsValidItem() const
{
	return IsValid(ItemInstance.Get()) && SlotIndex != INVALID_ITEM_INDEX && LeftTopIndex != INVALID_ITEM_INDEX;
}
