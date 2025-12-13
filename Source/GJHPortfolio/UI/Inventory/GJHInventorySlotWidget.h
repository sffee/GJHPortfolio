#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUITypes.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventorySlotWidget.generated.h"

class USizeBox;
class UGJHItemInstance;
class UGJHInventoryGridWidget;
class UBorder;

DECLARE_DELEGATE_TwoParams(FGJHOnDraggedInventoryItemWidget, class UGJHDraggedInventoryItemWidget* DraggedInventoryItemWidget, int32 SlotIndex);

UCLASS()
class GJHPORTFOLIO_API UGJHInventorySlotWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Slot;
	
private:
	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DefaultColor = FLinearColor(0.f, 0.f, 0.f, 0.f);
	
	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor HoveredColor = FLinearColor(0.87f, 0.56f, 0.063f, 0.75f);

	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DragValidPlacementColor = FLinearColor(0.43f, 0.87f, 0.036f, 0.75f);

	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DragInvalidPlacementColor = FLinearColor(1.f, 0.03f, 0.04f, 0.75f);
	
private:
	int32 SlotIndex = -1;
	TWeakObjectPtr<UGJHInventoryGridWidget> ParentGridWidget;

	int32 LeftTopIndex = -1;
	TWeakObjectPtr<UGJHItemInstance> ItemInstance;

public:
	FGJHOnDraggedInventoryItemWidget OnDragEnterInventoryItemWidget; 
	FGJHOnDraggedInventoryItemWidget OnDragLeaveInventoryItemWidget; 
	FGJHOnDraggedInventoryItemWidget OnDragCancelledInventoryItemWidget;
	FGJHOnDraggedInventoryItemWidget OnDropInventoryItemWidget;

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void SetData(const int32 InSlotIndex, UGJHInventoryGridWidget* InGridWidget);
	void ClearItem();
	
public:
	void SetSlotDefaultColor();
	void UpdateDraggedSlotColor(const FGJHDraggedInventoryItemResult InDraggedInventoryItemResult) const;

public:
	FORCEINLINE void SetLeftTopIndex(const int32 InLeftTopIndex) { LeftTopIndex = InLeftTopIndex; };
	FORCEINLINE void SetItemInstance(UGJHItemInstance* InItemInstance) { ItemInstance = InItemInstance; }
	
public:
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE int32 GetLeftTopIndex() const { return LeftTopIndex; }
	FORCEINLINE UGJHItemInstance* GetItemInstance() const { return ItemInstance.Get(); }
	FVector2D GetSize() const;
	
	bool IsValidItem() const;
};
