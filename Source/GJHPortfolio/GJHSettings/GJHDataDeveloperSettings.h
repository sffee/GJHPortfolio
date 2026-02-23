#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "GJHDataDeveloperSettings.generated.h"

class UGameplayEffect;
class UTexture2D;

USTRUCT()
struct FGJHStatusData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffect; 
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Icon;
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "GJH Data Settings"))
class GJHPORTFOLIO_API UGJHDataDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "DataTable", meta = (ForceInlineRow, Categories = "DataTable.Type"))
	TMap<FGameplayTag, TSoftObjectPtr<UDataTable>> DataTableMap;
	
	UPROPERTY(Config, EditAnywhere, Category = "Widget", meta = (ForceInlineRow, Categories = "Status"))
	TMap<FGameplayTag, FGJHStatusData> StatusMap;
};
