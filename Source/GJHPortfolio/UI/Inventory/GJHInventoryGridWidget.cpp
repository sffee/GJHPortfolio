#include "GJHInventoryGridWidget.h"

#include "GJHInventoryItemWidget.h"
#include "GJHInventorySlotWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/Player/Components/GJHInventoryComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Item/Definition/GJHItemDefinition.h"
#include "Item/Definition/GJHItemInstance.h"
#include "Library/GJHInventoryStatics.h"

void UGJHInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryComponent = UGJHInventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryGridSize = InventoryComponent.IsValid() ? InventoryComponent->GetGridSize() : FIntPoint();
	
	InitGrid();
}

void UGJHInventoryGridWidget::InitGrid()
{
	if (IsValid(GridPanel_Grid) == false || InventorySlotWidgetClass == nullptr || InventoryComponent.IsValid() == false)
		return;

	GridPanel_Grid->ClearChildren();
	Slots.Empty();
	Items.Empty();
	
	for (int32 i = 0; i < InventoryGridSize.X; ++i)
	{
		for (int32 j = 0; j < InventoryGridSize.Y; ++j)
		{
			UGJHInventorySlotWidget* SlotWidget = CreateWidget<UGJHInventorySlotWidget>(GetOwningPlayer(), InventorySlotWidgetClass);
			check(SlotWidget);

			Slots.Add(SlotWidget);

			const int32 SlotIndex = UGJHInventoryStatics::GetSlotIndexByCoordinate(i, j, InventoryGridSize.Y);
			SlotWidget->SetData(SlotIndex, this);

			if (UGridSlot* GridSlot = GridPanel_Grid->AddChildToGrid(SlotWidget, j, i); IsValid(GridSlot))
			{
				GridSlot->SetRowSpan(1);
				GridSlot->SetColumnSpan(1);
				GridSlot->SetLayer(0);
			}
		}
	}
	
	const TArray<TObjectPtr<UGJHItemInstance>>& ItemInstances = InventoryComponent->GetItemInstances();
	for (TObjectPtr<UGJHItemInstance> ItemInstance : ItemInstances)
	{
		UpdateSlot(ItemInstance, ItemInstance->GetInventorySlotIndex());
	}
}

bool UGJHInventoryGridWidget::FindEmptySlotIndex(TSubclassOf<UGJHItemDefinition> InItemDefinition, int32& OutSlotIndex)
{
	if (InventoryComponent.IsValid() == false)
		return false;
	
	const FIntPoint ItemGridSize = InItemDefinition->GetDefaultObject<UGJHItemDefinition>()->GetGridSize();

	for (UGJHInventorySlotWidget* SlotWidget : Slots)
	{
		if (IsInGridBounds(SlotWidget->GetSlotIndex(), ItemGridSize) == false)
			continue;

		if (IsSlotEmpty(SlotWidget->GetSlotIndex(), ItemGridSize) == false)
			continue;

		OutSlotIndex = SlotWidget->GetSlotIndex();
		return true;
	}

	return false;
}

UGJHItemInstance* UGJHInventoryGridWidget::AddItem(TSubclassOf<UGJHItemDefinition> InItemDefinition)
{
	int32 EmptySlotIndex = -1;
	if (FindEmptySlotIndex(InItemDefinition, EmptySlotIndex) == false)
		return nullptr;

	const bool bStackable = InItemDefinition->GetDefaultObject<UGJHItemDefinition>()->IsStackable();
	if (bStackable)
	{
		return nullptr;
	}

	UGJHItemInstance* ItemInstance = NewObject<UGJHItemInstance>(InventoryComponent->GetOwner());
	ItemInstance->SetItemDefinition(InItemDefinition);
	ItemInstance->SetInventorySlotIndex(EmptySlotIndex);

	UpdateSlot(ItemInstance, EmptySlotIndex);
	return ItemInstance;
}

bool UGJHInventoryGridWidget::IsSlotEmpty(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const
{	
	bool bSuccess = true;
	UGJHInventoryStatics::ForeachSlots(Slots, InSlotIndex, InItemGridSize, InventoryGridSize.Y, [&bSuccess](const UGJHInventorySlotWidget* SlotWidget) -> bool
	{
		if (SlotWidget->IsValidItem())
		{
			bSuccess = false;
			return false;
		}

		return true;
	});

	return bSuccess;
}

bool UGJHInventoryGridWidget::IsInGridBounds(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const
{
	if (InSlotIndex < 0 || Slots.Num() <= InSlotIndex)
		return false;

	const int32 EndColumn = (InSlotIndex / InventoryGridSize.Y) + InItemGridSize.X;
	const int32 EndRow = (InSlotIndex % InventoryGridSize.Y) + InItemGridSize.Y;

	return EndColumn <= InventoryGridSize.X && EndRow <= InventoryGridSize.Y;
}

void UGJHInventoryGridWidget::UpdateSlot(UGJHItemInstance* InItemInstance, const int32 InSlotIndex)
{
	if (InventoryItemWidgetClass == nullptr)
		return;

	const int32 Rows = InventoryGridSize.Y;
	const FIntPoint ItemGridSize = InItemInstance->GetItemDefinitionClass()->GetDefaultObject<UGJHItemDefinition>()->GetGridSize();
	const FIntPoint ItemCoordinate = UGJHInventoryStatics::GetCoordinateBySlotIndex(InSlotIndex, Rows);
	
	UGJHInventoryItemWidget* ItemWidget = CreateWidget<UGJHInventoryItemWidget>(GetOwningPlayer(), InventoryItemWidgetClass);
	ItemWidget->SetItemInstance(InItemInstance);
	
	if (UGridSlot* GridSlot = GridPanel_Grid->AddChildToGrid(ItemWidget, ItemCoordinate.Y, ItemCoordinate.X); IsValid(GridSlot))
	{
		GridSlot->SetColumnSpan(ItemGridSize.X);
		GridSlot->SetRowSpan(ItemGridSize.Y);
		GridSlot->SetLayer(1);
	}

	UGJHInventoryStatics::ForeachSlots(Slots, InSlotIndex, ItemGridSize, Rows, [SlotIndex = InSlotIndex, ItemInstance = InItemInstance](UGJHInventorySlotWidget* SlotWidget) -> bool
	{
		SlotWidget->SetItemInstance(ItemInstance);
		SlotWidget->SetLeftTopIndex(SlotIndex);
		
		return true;
	});
	
	Items.Add(ItemWidget);
}
