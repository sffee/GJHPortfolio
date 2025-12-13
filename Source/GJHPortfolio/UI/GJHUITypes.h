#pragma once

#include "CoreMinimal.h"

#include "GJHUITypes.generated.h"

USTRUCT()
struct FGJHDraggedInventoryItemResult
{
	GENERATED_BODY()

	int32 OverlappedItemCount = 0;
	int32 StartSlotIndex = 0;
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