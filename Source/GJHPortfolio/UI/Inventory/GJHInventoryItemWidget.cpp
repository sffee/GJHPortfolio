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
		PickupItem(false);
	}

	return FReply::Handled();
}

void UGJHInventoryItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Border_Slot->SetBrushColor(HoveredColor);
}

void UGJHInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	PickupItem(true);
}

void UGJHInventoryItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Border_Slot->SetBrushColor(DefaultColor);
}

void UGJHInventoryItemWidget::PickupItem(bool bInDrag)
{
	OnItemPickup.ExecuteIfBound(this, bInDrag);
}

void UGJHInventoryItemWidget::AddStack(const int32 InAddStack)
{
	Super::AddStack(InAddStack);

	TextBlock_Quantity->SetText(FText::AsNumber(ItemInstance->GetStack()));
}

void UGJHInventoryItemWidget::SetItemInstance(UGJHItemInstance* InItemInstance)
{
	Super::SetItemInstance(InItemInstance);
	
	if (ItemInstance.IsValid() == false)
		return;

	UTexture2D* ItemIcon = ItemInstance->GetItemDefinition()->GetIcon();
	if (IsValid(ItemIcon) == false)
		return;

	Image_ItemIcon->SetBrushFromTexture(ItemIcon);

	TextBlock_Quantity->SetVisibility(InItemInstance->IsStackable() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	TextBlock_Quantity->SetText(FText::AsNumber(InItemInstance->GetStack()));
	
	Border_Slot->SetBrushColor(DefaultColor);
}

void UGJHInventoryItemWidget::SetGridWidget(UGJHInventoryGridWidget* InGridWidget)
{
	ParentGridWidget = InGridWidget;
	ParentGridWidget->OnChangedDragState.AddUObject(this, &ThisClass::OnChangedDragState);
}

void UGJHInventoryItemWidget::AddItemStack(const int32 InAddStack) const
{
	ItemInstance->AddStack(InAddStack);
	TextBlock_Quantity->SetText(FText::AsNumber(ItemInstance->GetStack()));
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
