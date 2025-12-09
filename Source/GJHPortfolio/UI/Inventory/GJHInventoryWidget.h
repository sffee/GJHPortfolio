#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventoryWidget.generated.h"

class UGJHInventoryGridWidget;

UCLASS()
class GJHPORTFOLIO_API UGJHInventoryWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGJHInventoryGridWidget> InventoryGrid;

public:
	UGJHInventoryGridWidget* GetGridWidget() const { return InventoryGrid; }
};
