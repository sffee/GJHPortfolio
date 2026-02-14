#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GJHItemDefinition.generated.h"

class UTexture2D;

UCLASS(Blueprintable)
class GJHPORTFOLIO_API UGJHItemDefinition : public UObject
{
	GENERATED_BODY()

private:
	int32 ItemIndex = -1;
	
	UPROPERTY(EditAnywhere, Category = "GJH|Item", meta = (Categories = "Item.Type"))
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, Category = "GJH|Stack")
	bool bStackable = false;
	
	UPROPERTY(EditAnywhere, Category = "GJH|Stack", meta = (EditCondition = "bStackable == true"))
	int32 MaxStack = 1;
	
private:
	UPROPERTY(EditAnywhere, Category = "GJH|Inventory")
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, Category = "GJH|Inventory")
	FIntPoint GridSize;
	
public:
	FORCEINLINE void SetItemIndex(const int32 InItemIndex) { ItemIndex = InItemIndex; }
	FORCEINLINE int32 GetItemIndex() const { return ItemIndex; }
	
	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }
	FORCEINLINE FGameplayTag GetItemType() const { return ItemType; }
	FORCEINLINE FIntPoint GetGridSize() const { return GridSize; }

	FORCEINLINE bool IsStackable() const { return bStackable; }
	FORCEINLINE int32 GetMaxStack() const { return MaxStack; }
};
