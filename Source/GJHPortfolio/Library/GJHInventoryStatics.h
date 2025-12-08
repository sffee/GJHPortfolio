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
};
