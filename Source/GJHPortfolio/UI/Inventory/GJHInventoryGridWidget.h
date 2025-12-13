#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUITypes.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventoryGridWidget.generated.h"

class UGJHDraggedInventoryItemWidget;
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

public:
	FGJHOnChangedDragState OnChangedDragState;
	
private:
	TWeakObjectPtr<UGJHInventoryComponent> InventoryComponent;
	FIntPoint InventoryGridSize;

	TArray<TObjectPtr<UGJHInventorySlotWidget>> Slots;
	TArray<TObjectPtr<UGJHInventoryItemWidget>> Items;

	TWeakObjectPtr<UGJHDraggedInventoryItemWidget> DraggedInventoryItemWidget;
	int32 DraggedSlotIndex = -1;
	int32 LastDraggedStartSlotIndex = -1;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitGrid();

public:
	bool FindEmptySlotIndex(TSubclassOf<UGJHItemDefinition> InItemDefinition, int32& OutSlotIndex);
	
	UGJHItemInstance* AddItem(TSubclassOf<UGJHItemDefinition> InItemDefinition);

private:
	bool IsSlotEmpty(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const;
	bool IsInGridBounds(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const;

	FGJHDraggedInventoryItemResult GetDraggedInventoryItemResult(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const;
	EGJHSlotQuadrant CalcSlotQuadrant(const int32 InSlotIndex) const;
	int32 CalcDraggedStartSlotIndex(const int32 InSlotIndex, const FIntPoint& InItemGridSize, EGJHSlotQuadrant InSlotQuadrant) const;

private:
	void UpdateDraggedSlotWidget();
	void UpdateSlot(UGJHItemInstance* InItemInstance, const int32 InSlotIndex);
	void RestoreDraggedItem(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget);

	void ClearDraggedInventoryWidget(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget, int32 SlotIndex);

public:
	bool IsDragged() const;
	
private:
	void OnItemDragDetected(UGJHInventoryItemWidget* InventoryItemWidget);
	void OnDragEnterInventoryItem(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget, int32 SlotIndex);
	void OnDragLeaveInventoryItem(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget, int32 SlotIndex);
	void OnDragCancelledInventoryItem(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget, int32 SlotIndex);
	void OnDropInventoryItem(UGJHDraggedInventoryItemWidget* InDraggedInventoryItemWidget, int32 SlotIndex);
};
