#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUITypes.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventoryGridWidget.generated.h"

class UGJHPickupInventoryItemWidget;
class UGJHInventoryWidget;
class UGJHInventoryItemWidget;
class UGJHItemInstance;
class UGJHItemDefinition;
class UGridPanel;
class UGJHInventoryComponent;
class UGJHInventorySlotWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FGJHOnChangedDragState, bool bIsStart);

UCLASS()
class GJHPORTFOLIO_API UGJHInventoryGridWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> GridPanel_Grid;
	
private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	TSubclassOf<UGJHInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY(EditAnywhere, Category = "GJH")
	TSubclassOf<UGJHInventoryItemWidget> InventoryItemWidgetClass;

	UPROPERTY(EditAnywhere, Category = "GJH")
	TSubclassOf<UGJHPickupInventoryItemWidget> PickupInventoryItemWidgetClass;

public:
	FGJHOnChangedDragState OnChangedDragState;
	
private:
	TWeakObjectPtr<UGJHInventoryWidget> ParentInventoryWidget;
	TWeakObjectPtr<UGJHInventoryComponent> InventoryComponent;
	FIntPoint InventoryGridSize;

	TArray<TObjectPtr<UGJHInventorySlotWidget>> Slots;
	TArray<TObjectPtr<UGJHInventoryItemWidget>> Items;

	TWeakObjectPtr<UGJHPickupInventoryItemWidget> PickupInventoryItemWidget;
	int32 DraggedSlotIndex = -1;
	int32 LastDraggedStartSlotIndex = -1;
	bool bDraggedPickupItem = false;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitGrid();

public:
	bool FindEmptySlotIndex(TSubclassOf<UGJHItemDefinition> InItemDefinition, int32& OutSlotIndex);
	bool FindStackableSlotIndex(TSubclassOf<UGJHItemDefinition> InItemDefinition, FGJHInventoryStackableItemResult& OutStackableItemResult);

	void AddStackableItem(TSubclassOf<UGJHItemDefinition> InItemDefinition, const int32 InStack, TArray<TObjectPtr<UGJHItemInstance>>& OutItemInstances);
	UGJHItemInstance* AddEquipmentItem(TSubclassOf<UGJHItemDefinition> InItemDefinition);
	
	void RemoveItem(const int32 InSlotIndex);

private:
	UGJHItemInstance* CreateItemInstance(TSubclassOf<UGJHItemDefinition> InItemDefinition, const int32 InSlotIndex, const int32 InStack);
	
private:
	bool IsSlotEmpty(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const;
	bool IsInGridBounds(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const;

	FGJHDraggedInventoryItemResult GetDraggedInventoryItemResult(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const;
	EGJHSlotQuadrant CalcSlotQuadrant(const int32 InSlotIndex) const;
	int32 CalcDraggedStartSlotIndex(const int32 InSlotIndex, const FIntPoint& InItemGridSize, EGJHSlotQuadrant InSlotQuadrant) const;
	UGJHInventoryItemWidget* GetInventoryItemWidgetBySlotIndex(const int32 InSlotIndex);
	UGJHInventoryItemWidget* GetInventoryItemWidgetByItemInstance(const UGJHItemInstance* InItemInstance);

private:	
	void UpdateDraggedSlotWidget();
	void UpdateSlot(UGJHItemInstance* InItemInstance, const int32 InSlotIndex);

	void ClearPickupInventoryWidget(UGJHPickupInventoryItemWidget* InPickupInventoryItemWidget, int32 SlotIndex);
	void SwapPickupItem(UGJHPickupInventoryItemWidget* InPickupInventoryItemWidget, int32 SlotIndex);

	void CreatePickupInventoryItemWidget(UGJHInventoryItemWidget* InInventoryItemWidget);

public:
	bool IsDraggedPickupItem() const;

public:
	void SetParentWidget(UGJHInventoryWidget* InParentInventoryWidget);
	
private:
	void OnItemPickup(UGJHInventoryItemWidget* InInventoryItemWidget, bool bIsDrag);
	void OnDropPickupInventoryItem(UGJHInventorySlotWidget* InventorySlotWidget);
	void OnMouseEnterInventorySlotWidget(UGJHInventorySlotWidget* InventorySlotWidget, int32 SlotIndex);
	void OnMouseLeaveInventorySlotWidget(UGJHInventorySlotWidget* InventorySlotWidget, int32 SlotIndex);
};
