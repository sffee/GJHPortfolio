#include "GJHGameplayEffectExecutionCalculation.h"

#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "GameplayTag/GJHGameplayTag.h"

struct GJHDamageStatics
{
	//DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	//DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	
	GJHDamageStatics()
	{
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UGJHCharacterAttributeSet, Armor, Target, false);
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UGJHCharacterAttributeSet, ArmorPenetration, Source, false);
	}
};

void UGJHGameplayEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	const float Damage = EffectSpec.GetSetByCallerMagnitude(FGJHGameplayTag::Ability_SetByCaller_Damage(), false);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UGJHCharacterAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
