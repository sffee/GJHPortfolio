#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventoryWidget.generated.h"

class UCanvasPanel;
class UGJHPickupInventoryItemWidget;
class UGJHInventoryGridWidget;

UCLASS()
class GJHPORTFOLIO_API UGJHInventoryWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel_Root;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGJHInventoryGridWidget> InventoryGrid;

private:
	TWeakObjectPtr<UGJHPickupInventoryItemWidget> PickUpInventoryItemWidget;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void UpdatePickupWidget(const FGeometry& MyGeometry);
	
public:
	void PickupItem(UGJHPickupInventoryItemWidget* InPickupInventoryItemWidget);
	void DropPickupItem();
	
public:
	UGJHInventoryGridWidget* GetGridWidget() const { return InventoryGrid; }
};
