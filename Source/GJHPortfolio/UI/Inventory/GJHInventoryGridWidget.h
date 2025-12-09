#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventoryGridWidget.generated.h"

class UGJHInventoryItemWidget;
class UGJHItemInstance;
class UGJHItemDefinition;
class UGridPanel;
class UGJHInventoryComponent;
class UGJHInventorySlotWidget;

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

private:
	TWeakObjectPtr<UGJHInventoryComponent> InventoryComponent;
	FIntPoint InventoryGridSize;

	TArray<TObjectPtr<UGJHInventorySlotWidget>> Slots;
	TArray<TObjectPtr<UGJHInventoryItemWidget>> Items;

public:
	virtual void NativeConstruct() override;

	void InitGrid();

public:
	bool FindEmptySlotIndex(TSubclassOf<UGJHItemDefinition> InItemDefinition, int32& OutSlotIndex);
	
	UGJHItemInstance* AddItem(TSubclassOf<UGJHItemDefinition> InItemDefinition);

private:
	bool IsSlotEmpty(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const;
	bool IsInGridBounds(const int32 InSlotIndex, const FIntPoint& InItemGridSize) const;

private:
	void UpdateSlot(UGJHItemInstance* InItemInstance, const int32 InSlotIndex);
};
