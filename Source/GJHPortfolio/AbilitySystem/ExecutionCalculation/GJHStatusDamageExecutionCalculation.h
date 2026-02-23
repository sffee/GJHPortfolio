#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GJHStatusDamageExecutionCalculation.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHStatusDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGJHStatusDamageExecutionCalculation();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
private:
	float CalcStatusDamage(const FGameplayEffectCustomExecutionParameters& InExecutionParams, const FAggregatorEvaluateParameters& InEvaluationParameters, const FGameplayEffectSpec& InEffectSpec) const;
};
