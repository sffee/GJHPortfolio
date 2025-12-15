#include "GJHInventoryGridWidget.h"

#include "GJHInventoryItemWidget.h"
#include "GJHInventorySlotWidget.h"
#include "GJHInventoryWidget.h"
#include "GJHPickupInventoryItemWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/Player/Components/GJHInventoryComponent.h"
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

	if (PickupInventoryItemWidget.IsValid())
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

			SlotWidget->OnMouseEnterInventorySlotWidget.BindUObject(this, &ThisClass::OnMouseEnterInventorySlotWidget);
			SlotWidget->OnMouseLeaveInventorySlotWidget.BindUObject(this, &ThisClass::OnMouseLeaveInventorySlotWidget);
			SlotWidget->OnDropPickupInventoryItem.BindUObject(this, &ThisClass::OnDropPickupInventoryItem);

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

bool UGJHInventoryGridWidget::FindStackableSlotIndex(TSubclassOf<UGJHItemDefinition> InItemDefinition, FGJHInventoryStackableItemResult& OutStackableItemResult)
{
	if (InventoryComponent.IsValid() == false)
		return false;

	const int32 MaxStack = InItemDefinition->GetDefaultObject<UGJHItemDefinition>()->GetMaxStack();
	
	for (UGJHInventoryItemWidget* ItemWidget : Items)
	{
		if (ItemWidget->IsStackable() == false)
			continue;

		if (ItemWidget->GetItemDefinitionClass() != InItemDefinition)
			continue;

		int32 Remainder = MaxStack - ItemWidget->GetStack();
		if (Remainder == 0)
			continue;

		OutStackableItemResult.SlotIndex = ItemWidget->GetSlotIndex();
		OutStackableItemResult.Remainder = Remainder;
		OutStackableItemResult.InventoryItemWidget = ItemWidget;

		return true;
	}

	return false;
}

void UGJHInventoryGridWidget::RemoveItem(const int32 InSlotIndex)
{
	for (UGJHInventoryItemWidget* InventoryItemWidget : Items)
	{
		if (UGJHInventoryStatics::IsInCoordinateBounds(InventoryItemWidget->GetSlotIndex(), InSlotIndex, InventoryItemWidget->GetGridSize(), InventoryGridSize.Y))
		{
			Items.Remove(InventoryItemWidget);
			return;
		}
	}
}

UGJHItemInstance* UGJHInventoryGridWidget::CreateItemInstance(TSubclassOf<UGJHItemDefinition> InItemDefinition, const int32 InSlotIndex, const int32 InStack)
{
	UGJHItemInstance* ItemInstance = NewObject<UGJHItemInstance>(InventoryComponent->GetOwner());
	ItemInstance->SetItemDefinition(InItemDefinition);
	ItemInstance->SetInventorySlotIndex(InSlotIndex);
	ItemInstance->AddStack(InStack);

	UpdateSlot(ItemInstance, InSlotIndex);

	return ItemInstance;
}

void UGJHInventoryGridWidget::AddStackableItem(TSubclassOf<UGJHItemDefinition> InItemDefinition, const int32 InStack, TArray<TObjectPtr<UGJHItemInstance>>& OutItemInstances)
{
	const int32 MaxStack = InItemDefinition->GetDefaultObject<UGJHItemDefinition>()->GetMaxStack();
	int32 AddStack = InStack;

	while (0 < AddStack)
	{
		FGJHInventoryStackableItemResult StackableItemResult;
		if (FindStackableSlotIndex(InItemDefinition, StackableItemResult))
		{
			const int32 Amount = FMath::Min(AddStack, StackableItemResult.Remainder);
			StackableItemResult.InventoryItemWidget->AddItemStack(Amount);
			AddStack -= Amount;
			continue;
		}

		int32 EmptySlotIndex = -1;
		if (FindEmptySlotIndex(InItemDefinition, EmptySlotIndex) == false)
			break;

		const int32 Amount = FMath::Min(AddStack, MaxStack);
		AddStack -= Amount;
		OutItemInstances.Add(CreateItemInstance(InItemDefinition, EmptySlotIndex, Amount));
	}
}

UGJHItemInstance* UGJHInventoryGridWidget::AddEquipmentItem(TSubclassOf<UGJHItemDefinition> InItemDefinition)
{
	int32 EmptySlotIndex = -1;
	
	if (FindEmptySlotIndex(InItemDefinition, EmptySlotIndex) == false)
		return nullptr;
	
	return CreateItemInstance(InItemDefinition, EmptySlotIndex, 1);
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

UGJHInventoryItemWidget* UGJHInventoryGridWidget::GetInventoryItemWidgetBySlotIndex(const int32 InSlotIndex)
{
	for (UGJHInventoryItemWidget* InventoryItemWidget : Items)
	{
		if (UGJHInventoryStatics::IsInCoordinateBounds(InSlotIndex, InventoryItemWidget->GetSlotIndex(), InventoryItemWidget->GetGridSize(), InventoryGridSize.Y))
			return InventoryItemWidget;
	}

	return nullptr;
}

UGJHInventoryItemWidget* UGJHInventoryGridWidget::GetInventoryItemWidgetByItemInstance(const UGJHItemInstance* InItemInstance)
{
	for (UGJHInventoryItemWidget* InventoryItemWidget : Items)
	{
		if (InventoryItemWidget->GetItemInstance() == InItemInstance)
			return InventoryItemWidget;
	}

	return nullptr;
}

void UGJHInventoryGridWidget::UpdateDraggedSlotWidget()
{
	const FIntPoint ItemGridSize = PickupInventoryItemWidget->GetGridSize();

	if (DraggedSlotIndex != -1)
		int a = 0;

	const FGJHDraggedInventoryItemResult DraggedInventoryItemResult = GetDraggedInventoryItemResult(DraggedSlotIndex, ItemGridSize);
	if (LastDraggedStartSlotIndex != DraggedInventoryItemResult.StartSlotIndex)
	{
		UGJHInventoryStatics::ForeachSlots(Slots, LastDraggedStartSlotIndex, ItemGridSize, InventoryGridSize.Y, [](UGJHInventorySlotWidget* SlotWidget) -> bool
		{
			SlotWidget->SetSlotDefaultColor();
			return true;
		});
	}

	if (DraggedSlotIndex == -1)
		return;

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
	ItemWidget->OnItemPickup.BindUObject(this, &ThisClass::OnItemPickup);
	
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

void UGJHInventoryGridWidget::ClearPickupInventoryWidget(UGJHPickupInventoryItemWidget* InPickupInventoryItemWidget, int32 SlotIndex)
{
	const UGJHItemDefinition* ItemDefinition = InPickupInventoryItemWidget->GetItemInstance()->GetItemDefinition();
	const FIntPoint ItemGridSize = ItemDefinition->GetGridSize();

	const FGJHDraggedInventoryItemResult DraggedInventoryItemResult = GetDraggedInventoryItemResult(SlotIndex, ItemGridSize);
	UGJHInventoryStatics::ForeachSlots(Slots, SlotIndex, ItemGridSize, InventoryGridSize.Y, [&DraggedInventoryItemResult](UGJHInventorySlotWidget* SlotWidget) -> bool
	{
		SlotWidget->SetSlotDefaultColor();
		return true;
	});

	PickupInventoryItemWidget = nullptr;
	DraggedSlotIndex = -1;
	LastDraggedStartSlotIndex = -1;
}

void UGJHInventoryGridWidget::SwapPickupItem(UGJHPickupInventoryItemWidget* InPickupInventoryItemWidget, int32 SlotIndex)
{
	OnItemPickup(GetInventoryItemWidgetBySlotIndex(SlotIndex), false);
	UpdateSlot(InPickupInventoryItemWidget->GetItemInstance(), SlotIndex);
}

void UGJHInventoryGridWidget::CreatePickupInventoryItemWidget(UGJHInventoryItemWidget* InInventoryItemWidget)
{
	if (PickupInventoryItemWidgetClass == nullptr)
		return;

	if (PickupInventoryItemWidget.IsValid())
	{
		PickupInventoryItemWidget->RemoveFromParent();
		PickupInventoryItemWidget = nullptr;
	}
	
	UTexture2D* ItemIcon = InInventoryItemWidget->GetItemIcon(); 
	if (IsValid(ItemIcon) == false)
		return;
	
	const FIntPoint ItemGridSize = InInventoryItemWidget->GetGridSize();
	
	PickupInventoryItemWidget = CreateWidget<UGJHPickupInventoryItemWidget>(GetOwningPlayer(), PickupInventoryItemWidgetClass);
	PickupInventoryItemWidget->SetPrevSlotIndex(InInventoryItemWidget->GetSlotIndex());
	PickupInventoryItemWidget->SetItemIcon(ItemIcon);
	PickupInventoryItemWidget->SetSize(ItemGridSize.X * InInventoryItemWidget->GetWidthOverride(), ItemGridSize.Y * InInventoryItemWidget->GetHeightOverride());
	PickupInventoryItemWidget->SetItemInstance(InInventoryItemWidget->GetItemInstance());
}

bool UGJHInventoryGridWidget::IsDraggedPickupItem() const
{
	return PickupInventoryItemWidget.IsValid() && bDraggedPickupItem;
}

void UGJHInventoryGridWidget::SetParentWidget(UGJHInventoryWidget* InParentInventoryWidget)
{
	ParentInventoryWidget = InParentInventoryWidget;
}

void UGJHInventoryGridWidget::OnItemPickup(UGJHInventoryItemWidget* InInventoryItemWidget, bool bIsDrag)
{	
	CreatePickupInventoryItemWidget(InInventoryItemWidget);
	if (PickupInventoryItemWidget.IsValid() == false)
		return;
	
	const FIntPoint ItemGridSize = InInventoryItemWidget->GetGridSize();

	UGJHInventoryStatics::ForeachSlots(Slots, InInventoryItemWidget->GetSlotIndex(), ItemGridSize, InventoryGridSize.Y, [](UGJHInventorySlotWidget* SlotWidget) -> bool
	{
		SlotWidget->SetSlotDefaultColor();
		SlotWidget->ClearItem();
		return true;
	});

	RemoveItem(InInventoryItemWidget->GetSlotIndex());
	
	GridPanel_Grid->RemoveChild(InInventoryItemWidget);
	InInventoryItemWidget->RemoveFromParent();

	OnChangedDragState.Broadcast(true);

	ParentInventoryWidget->PickupItem(PickupInventoryItemWidget.Get());
	
	bDraggedPickupItem = true;
}

void UGJHInventoryGridWidget::OnDropPickupInventoryItem(UGJHInventorySlotWidget* InventorySlotWidget)
{
	if (PickupInventoryItemWidget.IsValid() == false)
		return;

	FGJHDraggedInventoryItemResult DraggedInventoryItemResult = GetDraggedInventoryItemResult(InventorySlotWidget->GetSlotIndex(), PickupInventoryItemWidget->GetGridSize());
	if (2 <= DraggedInventoryItemResult.OverlappedItemCount)
	{
		return;
	}
	else if (DraggedInventoryItemResult.OverlappedItemCount == 1)
	{
		if (PickupInventoryItemWidget->IsStackable())
		{
			if (PickupInventoryItemWidget->GetItemDefinition() == InventorySlotWidget->GetItemDefinition())
			{
				const int32 Remainder = InventorySlotWidget->GetStackRemainder();
				if (Remainder <= 0)
					return;
				
				if (Remainder < PickupInventoryItemWidget->GetStack())
				{
					PickupInventoryItemWidget->AddStack(-Remainder);
					GetInventoryItemWidgetBySlotIndex(InventorySlotWidget->GetSlotIndex())->AddStack(Remainder);
				}
				else if (PickupInventoryItemWidget->GetStack() <= Remainder)
				{
					GetInventoryItemWidgetBySlotIndex(InventorySlotWidget->GetSlotIndex())->AddStack(PickupInventoryItemWidget->GetStack());
					ClearPickupInventoryWidget(PickupInventoryItemWidget.Get(), LastDraggedStartSlotIndex);

					OnChangedDragState.Broadcast(false);
				}
				else
				{
					UpdateSlot(PickupInventoryItemWidget->GetItemInstance(), LastDraggedStartSlotIndex);
					InventoryComponent->UpdateItemSlotIndex(PickupInventoryItemWidget->GetItemInstance(), LastDraggedStartSlotIndex);
					ClearPickupInventoryWidget(PickupInventoryItemWidget.Get(), LastDraggedStartSlotIndex);

					OnChangedDragState.Broadcast(false);
				}
			}
			else
			{
				UpdateSlot(PickupInventoryItemWidget->GetItemInstance(), LastDraggedStartSlotIndex);
				InventoryComponent->UpdateItemSlotIndex(PickupInventoryItemWidget->GetItemInstance(), LastDraggedStartSlotIndex);
				ClearPickupInventoryWidget(PickupInventoryItemWidget.Get(), LastDraggedStartSlotIndex);
				
				OnChangedDragState.Broadcast(false);
			}
		}
		else
		{
			InventoryComponent->UpdateItemSlotIndex(PickupInventoryItemWidget->GetItemInstance(), LastDraggedStartSlotIndex);
			SwapPickupItem(PickupInventoryItemWidget.Get(), DraggedInventoryItemResult.StartSlotIndex);
		}
	}
	else
	{
		UpdateSlot(PickupInventoryItemWidget->GetItemInstance(), LastDraggedStartSlotIndex);
		InventoryComponent->UpdateItemSlotIndex(PickupInventoryItemWidget->GetItemInstance(), LastDraggedStartSlotIndex);
		ParentInventoryWidget->DropPickupItem();
		ClearPickupInventoryWidget(PickupInventoryItemWidget.Get(), LastDraggedStartSlotIndex);

		OnChangedDragState.Broadcast(false);
	}
}

void UGJHInventoryGridWidget::OnMouseEnterInventorySlotWidget(UGJHInventorySlotWidget* InventorySlotWidget, int32 SlotIndex)
{
	if (PickupInventoryItemWidget.IsValid() == false)
		return;

	DraggedSlotIndex = SlotIndex;
}

void UGJHInventoryGridWidget::OnMouseLeaveInventorySlotWidget(UGJHInventorySlotWidget* InventorySlotWidget, int32 SlotIndex)
{
	if (PickupInventoryItemWidget.IsValid() == false)
		return;

	DraggedSlotIndex = -1;
}
