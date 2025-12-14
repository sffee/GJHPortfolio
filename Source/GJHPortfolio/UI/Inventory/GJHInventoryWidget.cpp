#include "GJHInventoryWidget.h"

#include "GJHInventoryGridWidget.h"
#include "GJHPickupInventoryItemWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UGJHInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryGrid->SetParentWidget(this);
}

void UGJHInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PickUpInventoryItemWidget.IsValid())
		UpdatePickupWidget(MyGeometry);
}

void UGJHInventoryWidget::UpdatePickupWidget(const FGeometry& MyGeometry)
{
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(PickUpInventoryItemWidget.Get());
	if (IsValid(CanvasPanelSlot) == false)
		return;
	
	const FVector2D MousePosition = FSlateApplication::Get().GetCursorPos();
	const FVector2D LocalPosition = MyGeometry.AbsoluteToLocal(MousePosition);
	CanvasPanelSlot->SetPosition(LocalPosition);
}

void UGJHInventoryWidget::PickupItem(UGJHPickupInventoryItemWidget* InPickupInventoryItemWidget)
{
	PickUpInventoryItemWidget = InPickupInventoryItemWidget;
	if (PickUpInventoryItemWidget.IsValid())
	{
		if (UCanvasPanelSlot* CanvasPanelSlot = CanvasPanel_Root->AddChildToCanvas(PickUpInventoryItemWidget.Get()))
		{
			CanvasPanelSlot->SetAutoSize(true);
			CanvasPanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		}
	}
}

void UGJHInventoryWidget::DropPickupItem()
{
	CanvasPanel_Root->RemoveChild(PickUpInventoryItemWidget.Get());
	PickUpInventoryItemWidget = nullptr;
}
