#include "GJHInventoryEquipmentSlotWidget.h"

#include "Components/Image.h"

void UGJHInventoryEquipmentSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(Image_Slot) && IsValid(SlotImage))
		Image_Slot->SetBrushFromTexture(SlotImage);
}
