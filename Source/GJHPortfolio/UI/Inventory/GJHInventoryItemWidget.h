#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventoryItemWidget.generated.h"

class UGJHInventoryGridWidget;
class USizeBox;
class UGJHDraggedInventoryItemWidget;
class UBorder;
class UTextBlock;
class UImage;
class UGJHItemInstance;

DECLARE_DELEGATE_OneParam(FGJHOnItemDragDetected, class UGJHInventoryItemWidget* InventoryItemWidget);

UCLASS()
class GJHPORTFOLIO_API UGJHInventoryItemWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Quantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Slot;

private:
	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DefaultColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.1f);
	
	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor HoveredColor = FLinearColor(0.87f, 0.56f, 0.063f, 0.75f);

	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DragValidPlacementColor = FLinearColor(0.43f, 0.87f, 0.036f, 0.75f);

	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DragInvalidPlacementColor = FLinearColor(1.f, 0.03f, 0.04f, 0.75f);

public:
	FGJHOnItemDragDetected OnItemDragDetected;
	
private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	TSubclassOf<UGJHDraggedInventoryItemWidget> DraggedInventoryItemWidgetClass;
	
private:	
	TWeakObjectPtr<UGJHItemInstance> ItemInstance;
	int32 SlotIndex = -1;
	TWeakObjectPtr<UGJHInventoryGridWidget> ParentGridWidget;

	bool bMouseButtonDown = false;

public:
	virtual void NativeDestruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public:
	void SetItemInstance(UGJHItemInstance* InItemInstance);
	void SetGridWidget(UGJHInventoryGridWidget* InGridWidget);
	FORCEINLINE void SetSlotIndex(const int32 InSlotIndex) { SlotIndex = InSlotIndex; }
	
public:
	UGJHItemInstance* GetItemInstance() const { return ItemInstance.Get(); }
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }

private:
	void OnChangedDragState(bool bIsStart);
};
