#include "GJHInventoryGridWidget.h"

#include "GJHInventorySlotWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/Player/Components/GJHInventoryComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Library/GJHInventoryStatics.h"

void UGJHInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryComponent = UGJHInventoryStatics::GetInventoryComponent(GetOwningPlayer());
	
	InitGrid();
}

void UGJHInventoryGridWidget::InitGrid() const
{
	if (IsValid(GridPanel_Grid) == false || InventorySlotWidgetClass == nullptr || InventoryComponent.IsValid() == false)
		return;

	const FIntPoint GridSize = InventoryComponent->GetGridSize();
	const float SlotSize = InventorySlotWidgetClass->GetDefaultObject<UGJHInventorySlotWidget>()->GetSlotSize();

	GridPanel_Grid->ClearChildren();
	
	for (int32 i = 0; i < GridSize.X; ++i)
	{
		for (int32 j = 0; j < GridSize.Y; ++j)
		{
			UGJHInventorySlotWidget* SlotWidget = CreateWidget<UGJHInventorySlotWidget>(GetOwningPlayer(), InventorySlotWidgetClass);
			check(SlotWidget);

			if (UGridSlot* GridSlot = GridPanel_Grid->AddChildToGrid(SlotWidget, j, i); IsValid(GridSlot))
			{
				GridSlot->SetRowSpan(1);
				GridSlot->SetColumnSpan(1);
				GridSlot->SetLayer(0);
			}
		}
	}
}
