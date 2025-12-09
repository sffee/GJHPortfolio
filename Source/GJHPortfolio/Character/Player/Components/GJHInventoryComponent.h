#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GJHInventoryComponent.generated.h"

class UGJHItemInstance;

UCLASS()
class GJHPORTFOLIO_API UGJHInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	FIntPoint GridSize;

private:
	UPROPERTY()
	TArray<TObjectPtr<UGJHItemInstance>> ItemInstances;

public:
	UGJHInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	UGJHItemInstance* AddItem(const int32 InItemIndex);
	
public:
	FORCEINLINE FIntPoint GetGridSize() const { return GridSize; }
	FORCEINLINE const TArray<TObjectPtr<UGJHItemInstance>>& GetItemInstances() const { return ItemInstances; }
};
