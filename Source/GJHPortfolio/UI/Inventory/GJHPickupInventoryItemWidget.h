#pragma once

#include "CoreMinimal.h"
#include "GJHInventorySubWidgetBase.h"
#include "GJHPickupInventoryItemWidget.generated.h"

class UTextBlock;
class UGJHItemInstance;
class USizeBox;
class UImage;

UCLASS()
class GJHPORTFOLIO_API UGJHPickupInventoryItemWidget : public UGJHInventorySubWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Quantity;

private:
	int32 PrevSlotIndex = -1;

public:
	virtual void AddStack(const int32 InAddStack) override;
	
	void SetSize(const float InWidth, const float InHeight);
	void SetItemIcon(UTexture2D* InIcon);
	void SetPrevSlotIndex(const int32 InSlotIndex);
	virtual void SetItemInstance(UGJHItemInstance* InItemInstance) override;

public:
	FORCEINLINE int32 GetPrevSlotIndex() const { return PrevSlotIndex; }
};
