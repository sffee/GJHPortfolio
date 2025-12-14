#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GJHInventoryStatics.generated.h"

class UGJHInventoryComponent;

UCLASS()
class GJHPORTFOLIO_API UGJHInventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UGJHInventoryComponent* GetInventoryComponent(const UObject* InWorldContextObject);

public:
	static FIntPoint GetCoordinateBySlotIndex(const int32 InSlotIndex, const int32 InRows);
	static int32 GetSlotIndexByCoordinate(const int32 InX, const int32 InY, const int32 InRows);
	static int32 GetSlotIndexByCoordinate(const FIntPoint& InCoordinate, const int32 InRows);
	static bool IsInCoordinateBounds(const int32 InStartSlotIndex, const int32 InTargetSlotIndex, const FIntPoint& InCoordinate, const int32 InRows);

	template<typename T, typename FuncT>
	static void ForeachSlots(const TArray<T>& InSlots, int32 InStartIndex, const FIntPoint& InSlotRange, int32 InRows, const FuncT& InFunction);
};

template <typename T, typename FuncT>
void UGJHInventoryStatics::ForeachSlots(const TArray<T>& InSlots, int32 InStartIndex, const FIntPoint& InSlotRange, int32 InRows, const FuncT& InFunction)
{
	const FIntPoint StartCoordinates = UGJHInventoryStatics::GetCoordinateBySlotIndex(InStartIndex, InRows);
	
	for (int32 i = 0; i < InSlotRange.X; ++i)
	{
		for (int32 j = 0; j < InSlotRange.Y; ++j)
		{
			const FIntPoint Coordinates = StartCoordinates + FIntPoint(i, j);
			const int32 SlotIndex = UGJHInventoryStatics::GetSlotIndexByCoordinate(Coordinates, InRows);
			if (InSlots.IsValidIndex(SlotIndex))
			{
				if (InFunction(InSlots[SlotIndex]) == false)
					return;
			}
		}
	}
}
