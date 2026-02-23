#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GJHActorStatusWidgetComponent.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHActorStatusWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UGJHActorStatusWidgetComponent();

protected:
	virtual void BeginPlay() override;
};
