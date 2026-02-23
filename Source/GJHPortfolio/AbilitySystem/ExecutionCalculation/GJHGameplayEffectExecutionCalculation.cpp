#include "GJHGameplayEffectExecutionCalculation.h"

#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "GameplayTag/GJHGameplayTag.h"

void UGJHGameplayEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	const float Damage = EffectSpec.GetSetByCallerMagnitude(FGJHGameplayTag::Ability_SetByCaller_Damage(), false);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UGJHCharacterAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
