#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GJHInputConfigDataAsset.generated.h"

class UInputAction;

UCLASS()
class GJHPORTFOLIO_API UGJHInputConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Input|Default")
	TObjectPtr<UInputAction> MoveInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Input|Default")
	TObjectPtr<UInputAction> LookInputAction;

public:
	UPROPERTY(EditAnywhere, Category = "Input|UI")
	TObjectPtr<UInputAction> SkillTreeInputAction;

	UPROPERTY(EditAnywhere, Category = "Input|UI")
	TObjectPtr<UInputAction> InventoryInputAction;
	
public:
	UPROPERTY(EditAnywhere, Category = "Input|Ability", meta = (Categories = "Input", ForceInlineRow))
	TMap<FGameplayTag, TObjectPtr<UInputAction>> AbilityInputActionMap;
};
