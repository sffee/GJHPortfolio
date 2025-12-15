#pragma once

#include "CoreMinimal.h"
#include "GJHInventorySubWidgetBase.h"
#include "Item/Definition/GJHItemInstance.h"
#include "GJHInventoryItemWidget.generated.h"

class UGJHItemDefinition;
class UGJHPickupInventoryItemWidget;
class UGJHInventoryGridWidget;
class USizeBox;
class UBorder;
class UTextBlock;
class UImage;
class UGJHItemInstance;

DECLARE_DELEGATE_TwoParams(FGJHOnItemPickup, class UGJHInventoryItemWidget* InventoryItemWidget, bool bIsDrag);

UCLASS()
class GJHPORTFOLIO_API UGJHInventoryItemWidget : public UGJHInventorySubWidgetBase
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

public:
	FGJHOnItemPickup OnItemPickup;
	
private:	
	TWeakObjectPtr<UGJHInventoryGridWidget> ParentGridWidget;

	bool bMouseButtonDown = false;

public:
	virtual void NativeDestruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	void PickupItem(bool bInDrag);
	
public:
	virtual void AddStack(const int32 InAddStack) override;
	
	virtual void SetItemInstance(UGJHItemInstance* InItemInstance) override;
	void SetGridWidget(UGJHInventoryGridWidget* InGridWidget);

	void AddItemStack(const int32 InAddStack) const;
	
public:
	float GetWidthOverride() const;
	float GetHeightOverride() const;

private:
	void OnChangedDragState(bool bIsStart);
};
