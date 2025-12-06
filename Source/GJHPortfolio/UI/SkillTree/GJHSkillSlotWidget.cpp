#include "GJHSkillSlotWidget.h"

#include "GJHDraggedSkillSlotWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Image.h"
#include "Library/GJHDataStatics.h"

void UGJHSkillSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGJHSkillSlotWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

FReply UGJHSkillSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (SlotSkillIndex == -1)
		return FReply::Handled();

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bMouseButtonDown = true;
		Image_SkillIcon->SetBrushTintColor(GetSkillIconColor(ClickedColor));

		FReply Reply = FReply::Handled();
		Reply.DetectDrag(GetCachedWidget().ToSharedRef(), EKeys::LeftMouseButton);
		return Reply;
	}
	
	return FReply::Handled();
}

FReply UGJHSkillSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Image_SkillIcon->SetBrushTintColor(GetSkillIconColor(DefaultColor));

	if (bMouseButtonDown)
	{
		bMouseButtonDown = false;
		OnClickedImage_SkillSlot();
	}
	
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UGJHSkillSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bMouseButtonDown == false)
		Image_SkillIcon->SetBrushTintColor(GetSkillIconColor(HoveredColor));
	
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UGJHSkillSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bMouseButtonDown = false;
	Image_SkillIcon->SetBrushTintColor(GetSkillIconColor(DefaultColor));
	
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UGJHSkillSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (DraggedSkillSlotWidgetClass == nullptr)
		return;
	
	Image_SkillIcon->SetBrushTintColor(DefaultColor);

	UTexture2D* SkillIcon = Cast<UTexture2D>(Image_SkillIcon->GetBrush().GetResourceObject());
	if (IsValid(SkillIcon) == false)
		return;

	UGJHDraggedSkillSlotWidget* DraggedSkillSlotWidget = CreateWidget<UGJHDraggedSkillSlotWidget>(GetOwningPlayer(), DraggedSkillSlotWidgetClass);
	DraggedSkillSlotWidget->SetSlotSkillIndex(SlotSkillIndex);
	DraggedSkillSlotWidget->SetSkillIcon(SkillIcon);

	UDragDropOperation* DragDropOperation = NewObject<UDragDropOperation>(GetOwningPlayer());
	DragDropOperation->DefaultDragVisual = DraggedSkillSlotWidget;
	DragDropOperation->Pivot = EDragPivot::CenterCenter;

	OutOperation = DragDropOperation;
}

void UGJHSkillSlotWidget::SetSlotSkillIndex(int32 InSkillIndex)
{
	SlotSkillIndex = InSkillIndex;

	SetSlotSkillIcon(InSkillIndex);
}

void UGJHSkillSlotWidget::SetSlotSkillIcon(int32 InSkillIndex)
{
	UTexture2D* SkillIcon = UGJHDataStatics::GetSkillIcon(GetOwningPlayer(), InSkillIndex);
	if (IsValid(SkillIcon) == false)
		return;

	Image_SkillIcon->SetBrushTintColor(GetSkillIconColor(DefaultColor));
	Image_SkillIcon->SetBrushFromTexture(SkillIcon);
	Image_SkillIcon->SetVisibility(ESlateVisibility::Visible);
}

void UGJHSkillSlotWidget::ClearSlot()
{
	SlotSkillIndex = -1;
}

void UGJHSkillSlotWidget::OnClickedImage_SkillSlot()
{
	OnClickedSkillSlot.Broadcast(this);
}

FSlateColor UGJHSkillSlotWidget::GetSkillIconColor(const FSlateColor& InColor) const
{
	return FSlateColor(InColor.GetSpecifiedColor() * StateColor.GetSpecifiedColor());
}
