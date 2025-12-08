#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventoryGridWidget.generated.h"

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

	TWeakObjectPtr<UGJHInventoryComponent> InventoryComponent;

public:
	virtual void NativeConstruct() override;

	void InitGrid() const;
};
