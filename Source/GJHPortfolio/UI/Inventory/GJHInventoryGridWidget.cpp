#include "GJHInventoryGridWidget.h"

#include "GJHDraggedInventoryItemWidget.h"
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
#include "Library/GJHUIStatics.h"

void UGJHInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryComponent = UGJHInventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryGridSize = InventoryComponent.IsValid() ? InventoryComponent->GetGridSize() : FIntPoint();
	
	InitGrid();
}

void UGJHInventoryGridWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DraggedInventoryItemWidget.IsValid())
		UpdateDraggedSlotWidget();
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

			SlotWidget->OnDragEnterInventoryItemWidget.BindUObject(this, &ThisClass::OnDragEnterInventoryItem);
			SlotWidget->OnDragLeaveInventoryItemWidget.BindUObject(this, &ThisClass::OnDragLeaveInventoryItem);
			SlotWidget->OnDragCancelledInventoryItemWidget.BindUObject(this, &ThisClass::OnDragCancelledInventoryItem);
			SlotWidget->OnDropInventoryItemWidget.BindUObject(this, &ThisClass::OnDropInventoryItem);

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

FGJHDraggedInventoryItemResult UGJHInventoryGridWidget::GetDraggedInventoryItemResult(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const
{
	FGJHDraggedInventoryItemResult Result;

	const int32 DraggedStartSlotIndex = CalcDraggedStartSlotIndex(InSlotIndex, InItemGridSize, CalcSlotQuadrant(InSlotIndex));
	Result.StartSlotIndex = DraggedStartSlotIndex;

	TSet<int32> ValidItemSlotIndexes;
	UGJHInventoryStatics::ForeachSlots(Slots, DraggedStartSlotIndex, InItemGridSize, InventoryGridSize.Y, [&Result, &ValidItemSlotIndexes](UGJHInventorySlotWidget* SlotWidget) -> bool
	{
		if (SlotWidget->IsValidItem() && ValidItemSlotIndexes.Contains(SlotWidget->GetLeftTopIndex()) == false)
		{
			ValidItemSlotIndexes.Add(SlotWidget->GetLeftTopIndex());
			Result.OverlappedItemCount++;
		}
		
		return true;
	});

	return Result;
}

EGJHSlotQuadrant UGJHInventoryGridWidget::CalcSlotQuadrant(const int32 InSlotIndex) const
{
	EGJHSlotQuadrant Result = EGJHSlotQuadrant::None;
	if (Slots.IsValidIndex(InSlotIndex) == false)
		return Result;

	UGJHInventorySlotWidget* SlotWidget = Slots[InSlotIndex];
	const FVector2D WidgetViewportPosition = UGJHUIStatics::GetWidgetViewportPosition(SlotWidget, FVector2D(0.1f, 0.1f));
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	const FVector2D SlotWidgetSize = SlotWidget->GetSize();

	const float LocalPositionX = FMath::Fmod(MousePosition.X - WidgetViewportPosition.X, SlotWidgetSize.X);
	const float LocalPositionY = FMath::Fmod(MousePosition.Y - WidgetViewportPosition.Y, SlotWidgetSize.Y);

	const bool bIsTop = LocalPositionY < SlotWidgetSize.Y * 0.5f;
	const bool bIsLeft = LocalPositionX < SlotWidgetSize.X * 0.5f;

	if (bIsTop && bIsLeft)
		Result = EGJHSlotQuadrant::TopLeft;
	else if (bIsTop && bIsLeft == false)
		Result = EGJHSlotQuadrant::TopRight;
	else if (bIsTop == false && bIsLeft)
		Result = EGJHSlotQuadrant::BottomLeft;
	else if (bIsTop == false && bIsLeft == false)
		Result = EGJHSlotQuadrant::BottomRight;

	return Result;
}

int32 UGJHInventoryGridWidget::CalcDraggedStartSlotIndex(const int32 InSlotIndex, const FIntPoint& InItemGridSize, EGJHSlotQuadrant InSlotQuadrant) const
{
	FIntPoint Coordinate = UGJHInventoryStatics::GetCoordinateBySlotIndex(InSlotIndex, InventoryGridSize.Y);

	const int32 HasEvenWidth = InItemGridSize.X % 2 == 0 && Coordinate.X != InventoryGridSize.X - 1 ? 1 : 0;
	const int32 HasEvenHeight = InItemGridSize.Y % 2 == 0 && Coordinate.Y != InventoryGridSize.Y - 1 ? 1 : 0;
	
	FIntPoint ResultCoordinate;
	switch (InSlotQuadrant)
	{
	case EGJHSlotQuadrant::TopLeft:
		ResultCoordinate.X = FMath::Clamp(Coordinate.X - FMath::FloorToInt(0.5f * InItemGridSize.X), 0, InventoryGridSize.X - 1);
		ResultCoordinate.Y = FMath::Clamp(Coordinate.Y - FMath::FloorToInt(0.5f * InItemGridSize.Y), 0, InventoryGridSize.Y - 1);
		break;
	case EGJHSlotQuadrant::TopRight:
		ResultCoordinate.X = FMath::Clamp(Coordinate.X - FMath::FloorToInt(0.5f * InItemGridSize.X) + HasEvenWidth, 0, InventoryGridSize.X - 1);
		ResultCoordinate.Y = FMath::Clamp(Coordinate.Y - FMath::FloorToInt(0.5f * InItemGridSize.Y), 0, InventoryGridSize.Y - 1);
		break;
	case EGJHSlotQuadrant::BottomLeft:
		ResultCoordinate.X = FMath::Clamp(Coordinate.X - FMath::FloorToInt(0.5f * InItemGridSize.X), 0, InventoryGridSize.X - 1);
		ResultCoordinate.Y = FMath::Clamp(Coordinate.Y - FMath::FloorToInt(0.5f * InItemGridSize.Y) + HasEvenHeight, 0, InventoryGridSize.Y - 1);
		break;
	case EGJHSlotQuadrant::BottomRight:
		ResultCoordinate.X = FMath::Clamp(Coordinate.X - FMath::FloorToInt(0.5f * InItemGridSize.X) + HasEvenWidth, 0, InventoryGridSize.X - 1);
		ResultCoordinate.Y = FMath::Clamp(Coordinate.Y - FMath::FloorToInt(0.5f * InItemGridSize.Y) + HasEvenHeight, 0, InventoryGridSize.Y - 1);
		break;
	default:
		return -1;
	}

	return UGJHInventoryStatics::GetSlotIndexByCoordinate(ResultCoordinate, InventoryGridSize.Y);
}

void UGJHInventoryGridWidget::UpdateDraggedSlotWidget()
{
	const UGJHItemDefinition* ItemDefinition = DraggedInventoryItemWidget->GetItemInstance()->GetItemDefinition();
	const FIntPoint ItemGridSize = ItemDefinition->GetGridSize();

	const FGJHDraggedInventoryItemResult DraggedInventoryItemResult = GetDraggedInventoryItemResult(DraggedSlotIndex, ItemGridSize);
	if (LastDraggedStartSlotIndex != DraggedInventoryItemResult.StartSlotIndex)
	{
		UGJHInventoryStatics::ForeachSlots(Slots, LastDraggedStartSlotIndex, ItemGridSize, InventoryGridSize.Y, [](UGJHInventorySlotWidget* SlotWidget) -> bool
		{
			SlotWidget->SetSlotDefaultColor();
			return true;
		});
	}
	
	UGJHInventoryStatics::ForeachSlots(Slots, DraggedInventoryItemResult.StartSlotIndex, ItemGridSize, InventoryGridSize.Y, [&DraggedInventoryItemResult](UGJHInventorySlotWidget* SlotWidget) -> bool
	{
		SlotWidget->UpdateDraggedSlotColor(DraggedInventoryItemResult);
		return true;
	});

	LastDraggedStartSlotIndex = DraggedInventoryItemResult.StartSlotIndex;
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
	ItemWidget->SetSlotIndex(InSlotIndex);
	ItemWidget->SetGridWidget(this);
	ItemWidget->OnItemDragDetected.BindUObject(this, &ThisClass::OnItemDragDetected);
	
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

void UGJHInventoryGridWidget::RestoreDraggedItem(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget)
{
	UpdateSlot(InDraggedInventoryItemWidget->GetItemInstance(), InDraggedInventoryItemWidget->GetPrevSlotIndex());
}

void UGJHInventoryGridWidget::ClearDraggedInventoryWidget(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget, int32 SlotIndex)
{
	const UGJHItemDefinition* ItemDefinition = InDraggedInventoryItemWidget->GetItemInstance()->GetItemDefinition();
	const FIntPoint ItemGridSize = ItemDefinition->GetGridSize();

	const FGJHDraggedInventoryItemResult DraggedInventoryItemResult = GetDraggedInventoryItemResult(SlotIndex, ItemGridSize);
	UGJHInventoryStatics::ForeachSlots(Slots, SlotIndex, ItemGridSize, InventoryGridSize.Y, [&DraggedInventoryItemResult](UGJHInventorySlotWidget* SlotWidget) -> bool
	{
		SlotWidget->SetSlotDefaultColor();
		return true;
	});

	DraggedInventoryItemWidget = nullptr;
	DraggedSlotIndex = -1;
	LastDraggedStartSlotIndex = -1;
}

bool UGJHInventoryGridWidget::IsDragged() const
{
	return DraggedInventoryItemWidget.IsValid();
}

void UGJHInventoryGridWidget::OnItemDragDetected(UGJHInventoryItemWidget* InventoryItemWidget)
{
	const UGJHItemDefinition* ItemDefinition = InventoryItemWidget->GetItemInstance()->GetItemDefinition();
	const FIntPoint ItemGridSize = ItemDefinition->GetGridSize();

	UGJHInventoryStatics::ForeachSlots(Slots, InventoryItemWidget->GetSlotIndex(), ItemGridSize, InventoryGridSize.Y, [](UGJHInventorySlotWidget* SlotWidget) -> bool
	{
		SlotWidget->ClearItem();
		return true;
	});
	
	GridPanel_Grid->RemoveChild(InventoryItemWidget);
	InventoryItemWidget->RemoveFromParent();

	OnChangedDragState.Broadcast(true);
}

void UGJHInventoryGridWidget::OnDragEnterInventoryItem(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget, int32 SlotIndex)
{
	DraggedInventoryItemWidget = InDraggedInventoryItemWidget;
	DraggedSlotIndex = SlotIndex;
}

void UGJHInventoryGridWidget::OnDragLeaveInventoryItem(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget, int32 SlotIndex)
{
	ClearDraggedInventoryWidget(InDraggedInventoryItemWidget, LastDraggedStartSlotIndex);
}

void UGJHInventoryGridWidget::OnDragCancelledInventoryItem(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget, int32 SlotIndex)
{	
	ClearDraggedInventoryWidget(InDraggedInventoryItemWidget, LastDraggedStartSlotIndex);
}

void UGJHInventoryGridWidget::OnDropInventoryItem(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget, int32 SlotIndex)
{
	if (IsSlotEmpty(LastDraggedStartSlotIndex, InDraggedInventoryItemWidget->GetGridSize()) == false)
	{
		RestoreDraggedItem(InDraggedInventoryItemWidget);
	}
	else
	{
		UpdateSlot(InDraggedInventoryItemWidget->GetItemInstance(), LastDraggedStartSlotIndex);
		InventoryComponent->UpdateItemSlotIndex(InDraggedInventoryItemWidget->GetItemInstance(), LastDraggedStartSlotIndex);
	}
	
	ClearDraggedInventoryWidget(InDraggedInventoryItemWidget, LastDraggedStartSlotIndex);

	OnChangedDragState.Broadcast(false);
}
