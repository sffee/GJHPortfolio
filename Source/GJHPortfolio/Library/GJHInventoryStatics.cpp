#include "GJHInventoryStatics.h"

#include "GJHGameplayStatics.h"
#include "Character/Player/GJHPlayerController.h"
#include "Character/Player/Components/GJHInventoryComponent.h"

UGJHInventoryComponent* UGJHInventoryStatics::GetInventoryComponent(const UObject* InWorldContextObject)
{
	AGJHPlayerController* PC = UGJHGameplayStatics::GetGJHPlayerController(InWorldContextObject);
	return IsValid(PC) ? PC->GetInventoryComponent() : nullptr;
}
