#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHInventorySlotWidget.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHInventorySlotWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	float SlotSize = 54.f;

public:
	FORCEINLINE float GetSlotSize() const { return SlotSize; }
};
