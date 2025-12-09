#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventorySlotWidget.generated.h"

class UGJHItemInstance;
class UGJHInventoryGridWidget;
class UBorder;

UCLASS()
class GJHPORTFOLIO_API UGJHInventorySlotWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
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
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetData(const int32 InSlotIndex, UGJHInventoryGridWidget* InGridWidget);

private:
	void SetSlotColor(const FLinearColor& InColor) const;

public:
	FORCEINLINE void SetLeftTopIndex(const int32 InLeftTopIndex) { LeftTopIndex = InLeftTopIndex; };
	FORCEINLINE void SetItemInstance(UGJHItemInstance* InItemInstance) { ItemInstance = InItemInstance; }
	
public:
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE int32 GetLeftTopIndex() const { return LeftTopIndex; }
	FORCEINLINE UGJHItemInstance* GetItemInstance() const { return ItemInstance.Get(); }
	
	bool IsValidItem() const;
};
