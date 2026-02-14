#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "GJHDataDeveloperSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "GJH Data Settings"))
class GJHPORTFOLIO_API UGJHDataDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "DataTable", meta = (ForceInlineRow, Categories = "DataTable.Type"))
	TMap<FGameplayTag, TSoftObjectPtr<UDataTable>> DataTableMap;
};
