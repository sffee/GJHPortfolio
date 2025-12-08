#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GJHInventoryComponent.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	FIntPoint GridSize;
	
public:
	UGJHInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE FIntPoint GetGridSize() const { return GridSize; }
};
