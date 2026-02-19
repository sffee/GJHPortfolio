#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GJHFileStatics.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHFileStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static void ForeachFileInDirectory(const FString& InDirectory, TFunctionRef<void(const FString&)> InFunc);
};
