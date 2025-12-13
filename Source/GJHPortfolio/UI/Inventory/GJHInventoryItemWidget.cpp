#include "GJHInventoryItemWidget.h"

#include "GJHDraggedInventoryItemWidget.h"
#include "GJHInventoryGridWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Item/Definition/GJHItemInstance.h"

void UGJHInventoryItemWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (ParentGridWidget.IsValid())
		ParentGridWidget->OnChangedDragState.RemoveAll(this);
}

FReply UGJHInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bMouseButtonDown = true;

		FReply Reply = FReply::Handled();
		Reply.DetectDrag(GetCachedWidget().ToSharedRef(), EKeys::LeftMouseButton);
		return Reply;
	}
	
	return FReply::Handled();
}

FReply UGJHInventoryItemWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bMouseButtonDown)
	{
		bMouseButtonDown = false;
		// TODO : 아이템 들기
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UGJHInventoryItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Border_Slot->SetBrushColor(HoveredColor);
}

void UGJHInventoryItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Border_Slot->SetBrushColor(DefaultColor);
}

void UGJHInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (DraggedInventoryItemWidgetClass == nullptr)
		return;
	
	UTexture2D* ItemIcon = Cast<UTexture2D>(Image_ItemIcon->GetBrush().GetResourceObject());
	if (IsValid(ItemIcon) == false)
		return;
	
	const FIntPoint ItemGridSize = ItemInstance->GetItemDefinition()->GetGridSize();
	
	UGJHDraggedInventoryItemWidget* DraggedInventoryItemWidget = CreateWidget<UGJHDraggedInventoryItemWidget>(GetOwningPlayer(), DraggedInventoryItemWidgetClass);
	DraggedInventoryItemWidget->SetPrevSlotIndex(SlotIndex);
	DraggedInventoryItemWidget->SetItemIcon(ItemIcon);
	DraggedInventoryItemWidget->SetSize(ItemGridSize.X * SizeBox_Root->GetWidthOverride(), ItemGridSize.Y * SizeBox_Root->GetHeightOverride());
	DraggedInventoryItemWidget->SetItemInstance(ItemInstance.Get());
	
	UDragDropOperation* DragDropOperation = NewObject<UDragDropOperation>(GetOwningPlayer());
	DragDropOperation->DefaultDragVisual = DraggedInventoryItemWidget;
	DragDropOperation->Pivot = EDragPivot::CenterCenter;
	
	OutOperation = DragDropOperation;

	OnItemDragDetected.ExecuteIfBound(this);
}

void UGJHInventoryItemWidget::SetItemInstance(UGJHItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
	if (ItemInstance.IsValid() == false)
		return;

	UTexture2D* ItemIcon = ItemInstance->GetItemDefinition()->GetIcon();
	if (IsValid(ItemIcon) == false)
		return;

	Image_ItemIcon->SetBrushFromTexture(ItemIcon);

	TextBlock_Quantity->SetVisibility(ESlateVisibility::Hidden);
	Border_Slot->SetBrushColor(DefaultColor);
}

void UGJHInventoryItemWidget::SetGridWidget(UGJHInventoryGridWidget* InGridWidget)
{
	ParentGridWidget = InGridWidget;
	ParentGridWidget->OnChangedDragState.AddUObject(this, &ThisClass::OnChangedDragState);
}

void UGJHInventoryItemWidget::OnChangedDragState(bool bIsStart)
{
	SizeBox_Root->SetVisibility(bIsStart ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
}
