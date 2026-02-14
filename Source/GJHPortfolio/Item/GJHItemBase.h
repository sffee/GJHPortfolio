#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GJHItemBase.generated.h"

class UGJHItemDefinition;

UCLASS(Abstract)
class GJHPORTFOLIO_API UGJHItemBase : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	TObjectPtr<UGJHItemDefinition> ItemDefinition;

private:
	UPROPERTY(VisibleAnywhere, Category = "GJH|Item")
	FGameplayTag ItemType;

public:
	FORCEINLINE void SetItemDefinition(UGJHItemDefinition* InItemDefinition) { ItemDefinition = InItemDefinition; }
	
	FORCEINLINE UGJHItemDefinition* GetItemDefinition() { return ItemDefinition; }
	FORCEINLINE FGameplayTag GetItemType() const { return ItemType; }
};
