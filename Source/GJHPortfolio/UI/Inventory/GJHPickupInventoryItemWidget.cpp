#include "GJHPickupInventoryItemWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Item/Definition/GJHItemInstance.h"

void UGJHPickupInventoryItemWidget::AddStack(const int32 InAddStack)
{
	Super::AddStack(InAddStack);

	TextBlock_Quantity->SetText(FText::AsNumber(ItemInstance->GetStack()));
}

void UGJHPickupInventoryItemWidget::SetSize(const float InWidth, const float InHeight)
{
	SizeBox_Root->SetWidthOverride(InWidth);
	SizeBox_Root->SetHeightOverride(InHeight);
}

void UGJHPickupInventoryItemWidget::SetItemIcon(UTexture2D* InIcon)
{
	Image_ItemIcon->SetBrushFromTexture(InIcon);
}

void UGJHPickupInventoryItemWidget::SetPrevSlotIndex(const int32 InSlotIndex)
{
	PrevSlotIndex = InSlotIndex;
}

void UGJHPickupInventoryItemWidget::SetItemInstance(UGJHItemInstance* InItemInstance)
{
	Super::SetItemInstance(InItemInstance);
	
	TextBlock_Quantity->SetVisibility(ItemInstance->IsStackable() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	TextBlock_Quantity->SetText(FText::AsNumber(ItemInstance->GetStack()));
}
