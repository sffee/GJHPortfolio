#include "GJHInventorySlotWidget.h"

#include "Components/Border.h"
#include "GJHInventoryGridWidget.h"
#include "Item/Definition/GJHItemInstance.h"

void UGJHInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	SetSlotColor(HoveredColor);
}

void UGJHInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	SetSlotColor(DefaultColor);
}

FReply UGJHInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UGJHInventorySlotWidget::SetData(const int32 InSlotIndex, UGJHInventoryGridWidget* InGridWidget)
{
	SlotIndex = InSlotIndex;
	ParentGridWidget = InGridWidget;

	SetSlotColor(DefaultColor);
}

void UGJHInventorySlotWidget::SetSlotColor(const FLinearColor& InColor) const
{
	Border_Slot->SetBrushColor(InColor);
}

bool UGJHInventorySlotWidget::IsValidItem() const
{
	return IsValid(ItemInstance.Get()) && SlotIndex != INVALID_ITEM_INDEX && LeftTopIndex != INVALID_ITEM_INDEX;
}
