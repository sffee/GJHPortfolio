#include "GJHInventoryItemWidget.h"

#include "GJHInventoryGridWidget.h"
#include "GJHPickupInventoryItemWidget.h"
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
		PickupItem();
	}

	return FReply::Handled();
}

void UGJHInventoryItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Border_Slot->SetBrushColor(HoveredColor);
}

FReply UGJHInventoryItemWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bMouseButtonDown)
	{
		bMouseButtonDown = false;
		PickupItem();
	}
	
	return FReply::Handled();
}

void UGJHInventoryItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Border_Slot->SetBrushColor(DefaultColor);
}

void UGJHInventoryItemWidget::PickupItem()
{
	OnItemPickup.ExecuteIfBound(this);
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

FIntPoint UGJHInventoryItemWidget::GetGridSize() const
{
	return ItemInstance.IsValid() ? ItemInstance->GetItemDefinition()->GetGridSize() : FIntPoint::ZeroValue;
}

UTexture2D* UGJHInventoryItemWidget::GetItemIcon() const
{
	return Cast<UTexture2D>(Image_ItemIcon->GetBrush().GetResourceObject());
}

float UGJHInventoryItemWidget::GetWidthOverride() const
{
	return SizeBox_Root->GetWidthOverride();
}

float UGJHInventoryItemWidget::GetHeightOverride() const
{
	return SizeBox_Root->GetHeightOverride();
}

void UGJHInventoryItemWidget::OnChangedDragState(bool bIsStart)
{
	SizeBox_Root->SetVisibility(bIsStart ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
}
