#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GJHAbilitySystemGlobals.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

private:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
