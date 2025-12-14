#include "GJHPickupInventoryItemWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Item/Definition/GJHItemInstance.h"

void UGJHPickupInventoryItemWidget::SetSize(const float InWidth, const float InHeight)
{
	SizeBox_Root->SetWidthOverride(InWidth);
	SizeBox_Root->SetHeightOverride(InHeight);
}

void UGJHPickupInventoryItemWidget::SetItemIcon(UTexture2D* InIcon)
{
	Image_Item->SetBrushFromTexture(InIcon);
}

void UGJHPickupInventoryItemWidget::SetPrevSlotIndex(const int32 InSlotIndex)
{
	PrevSlotIndex = InSlotIndex;
}

void UGJHPickupInventoryItemWidget::SetItemInstance(UGJHItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
}

FIntPoint UGJHPickupInventoryItemWidget::GetGridSize() const
{
	return ItemInstance.IsValid() ? ItemInstance->GetItemDefinition()->GetGridSize() : FIntPoint::ZeroValue;
}
