#pragma once

#include "CoreMinimal.h"

#include "GJHUITypes.generated.h"

class UGJHInventoryItemWidget;

USTRUCT()
struct FGJHDraggedInventoryItemResult
{
	GENERATED_BODY()

	int32 OverlappedItemCount = 0;
	int32 StartSlotIndex = 0;
};

USTRUCT()
struct FGJHInventoryStackableItemResult
{
	GENERATED_BODY()

	int32 SlotIndex = 0;
	int32 Remainder = 0;
	TWeakObjectPtr<UGJHInventoryItemWidget> InventoryItemWidget;
};

UENUM(BlueprintType)
enum class EGJHSlotQuadrant : uint8
{
	None,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
};