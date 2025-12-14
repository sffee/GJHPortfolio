#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHPickupInventoryItemWidget.generated.h"

class UGJHItemInstance;
class USizeBox;
class UImage;

UCLASS()
class GJHPORTFOLIO_API UGJHPickupInventoryItemWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Item;

private:
	int32 PrevSlotIndex = -1;
	TWeakObjectPtr<UGJHItemInstance> ItemInstance;

public:
	void SetSize(const float InWidth, const float InHeight);
	void SetItemIcon(UTexture2D* InIcon);
	void SetPrevSlotIndex(const int32 InSlotIndex);
	void SetItemInstance(UGJHItemInstance* InItemInstance);

public:
	FORCEINLINE int32 GetPrevSlotIndex() const { return PrevSlotIndex; }
	FORCEINLINE UGJHItemInstance* GetItemInstance() const { return ItemInstance.Get(); }
	FIntPoint GetGridSize() const;
};
