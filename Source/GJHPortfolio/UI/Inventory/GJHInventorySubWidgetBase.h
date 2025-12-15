#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventorySubWidgetBase.generated.h"

class UGJHItemDefinition;
class UGJHItemInstance;

UCLASS(Abstract)
class GJHPORTFOLIO_API UGJHInventorySubWidgetBase : public UGJHUserWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DefaultColor = FLinearColor(0.f, 0.f, 0.f, 0.f);
	
	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor HoveredColor = FLinearColor(0.43f, 0.87f, 0.036f, 0.75f);

	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DragEmptyPlacementColor = FLinearColor(0.43f, 0.87f, 0.036f, 0.75f);
	
	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DragValidPlacementColor = FLinearColor(0.87f, 0.56f, 0.063f, 0.75f);

	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FLinearColor DragInvalidPlacementColor = FLinearColor(1.f, 0.03f, 0.04f, 0.75f);

protected:
	TWeakObjectPtr<UGJHItemInstance> ItemInstance;
	
	int32 SlotIndex = -1;
	int32 LeftTopIndex = -1;

public:
	virtual void AddStack(const int32 InAddStack);
	
	FORCEINLINE void SetSlotIndex(int32 InSlotIndex) { SlotIndex = InSlotIndex; }
	FORCEINLINE void SetLeftTopIndex(const int32 InLeftTopIndex) { LeftTopIndex = InLeftTopIndex; };
	virtual void SetItemInstance(UGJHItemInstance* InItemInstance);
	
public:
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE int32 GetLeftTopIndex() const { return LeftTopIndex; }
	FORCEINLINE UGJHItemInstance* GetItemInstance() const { return ItemInstance.Get(); }

	UGJHItemDefinition* GetItemDefinition() const;
	TSubclassOf<UGJHItemDefinition> GetItemDefinitionClass() const;
	int32 GetStack() const;
	FIntPoint GetGridSize() const;
	UTexture2D* GetItemIcon() const;
	int32 GetStackRemainder() const;

	bool IsValidItem() const;
	bool IsStackable() const;
};

