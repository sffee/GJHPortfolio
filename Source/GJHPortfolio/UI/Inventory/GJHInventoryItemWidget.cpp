#include "GJHInventoryItemWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/Definition/GJHItemInstance.h"

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
