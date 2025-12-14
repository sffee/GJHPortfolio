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

bool UGJHInventoryStatics::IsInCoordinateBounds(const int32 InStartSlotIndex, const int32 InTargetSlotIndex, const FIntPoint& InCoordinate, const int32 InRows)
{
	const FIntPoint StartCoordinate = GetCoordinateBySlotIndex(InStartSlotIndex, InRows);
	const FIntPoint TargetCoordinate = GetCoordinateBySlotIndex(InTargetSlotIndex, InRows);

	const FIntRect StartRect(StartCoordinate.X, StartCoordinate.Y, StartCoordinate.X + InCoordinate.X, StartCoordinate.Y + InCoordinate.Y);
	const FIntRect TargetRect(TargetCoordinate.X, TargetCoordinate.Y, TargetCoordinate.X + InCoordinate.X, TargetCoordinate.Y + InCoordinate.Y);

	return StartRect.Intersect(TargetRect);
}
