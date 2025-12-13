#include "GJHDraggedInventoryItemWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Item/Definition/GJHItemInstance.h"

void UGJHDraggedInventoryItemWidget::SetSize(const float InWidth, const float InHeight)
{
	SizeBox_Root->SetWidthOverride(InWidth);
	SizeBox_Root->SetHeightOverride(InHeight);
}

void UGJHDraggedInventoryItemWidget::SetItemIcon(UTexture2D* InIcon)
{
	Image_Item->SetBrushFromTexture(InIcon);
}

void UGJHDraggedInventoryItemWidget::SetPrevSlotIndex(const int32 InSlotIndex)
{
	PrevSlotIndex = InSlotIndex;
}

void UGJHDraggedInventoryItemWidget::SetItemInstance(UGJHItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
}

FIntPoint UGJHDraggedInventoryItemWidget::GetGridSize() const
{
	return ItemInstance.IsValid() ? ItemInstance->GetItemDefinition()->GetGridSize() : FIntPoint::ZeroValue;
}
