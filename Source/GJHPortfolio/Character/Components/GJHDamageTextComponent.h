#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/WidgetComponent.h"
#include "GJHDamageTextComponent.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UGJHDamageTextComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetDamage(float InDamage, const FGameplayTag& InStatusTag);
};
