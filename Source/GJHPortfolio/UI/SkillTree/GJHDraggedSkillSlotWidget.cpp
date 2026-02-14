#include "GJHDraggedSkillSlotWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"

void UGJHDraggedSkillSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGJHDraggedSkillSlotWidget::SetSize(float InWidth, float InHeight)
{
	SizeBox_Root->SetWidthOverride(InWidth);
	SizeBox_Root->SetHeightOverride(InHeight);
}

void UGJHDraggedSkillSlotWidget::SetSkillIcon(UTexture2D* InTexture)
{
	Image_SkillIcon->SetBrushFromTexture(InTexture);
}

void UGJHDraggedSkillSlotWidget::SetSlotSkillIndex(int32 InSkillIndex)
{
	SlotSkillIndex = InSkillIndex;
}
