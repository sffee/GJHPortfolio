#include "GJHInventoryStatics.h"

#include "GJHGameplayStatics.h"
#include "Character/Player/GJHPlayerController.h"
#include "Character/Player/Components/GJHInventoryComponent.h"

UGJHInventoryComponent* UGJHInventoryStatics::GetInventoryComponent(const UObject* InWorldContextObject)
{
	AGJHPlayerController* PC = UGJHGameplayStatics::GetGJHPlayerController(InWorldContextObject);
	return IsValid(PC) ? PC->GetInventoryComponent() : nullptr;
}

FIntPoint UGJHInventoryStatics::GetCoordinateBySlotIndex(const int32 InSlotIndex, const int32 InRows)
{
	FIntPoint Coordinate;
	Coordinate.X = InSlotIndex / InRows;
	Coordinate.Y = InSlotIndex % InRows;
	
	return Coordinate;
}

int32 UGJHInventoryStatics::GetSlotIndexByCoordinate(const int32 InX, const int32 InY, const int32 InRows)
{
	return GetSlotIndexByCoordinate(FIntPoint(InX, InY), InRows);
}

int32 UGJHInventoryStatics::GetSlotIndexByCoordinate(const FIntPoint& InCoordinate, const int32 InRows)
{
	return InCoordinate.X * InRows + InCoordinate.Y;
}
