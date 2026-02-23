#include "GJHStatusDamageExecutionCalculation.h"

#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "GameplayTag/GJHGameplayTag.h"

struct GJHStatusDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusDamageRatio);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusDamageResist);
	
	GJHStatusDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGJHCharacterAttributeSet, MaxHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGJHCharacterAttributeSet, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGJHCharacterAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGJHCharacterAttributeSet, StatusDamageRatio, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGJHCharacterAttributeSet, StatusDamageResist, Target, false);
	}
};

static const GJHStatusDamageStatics& StatusDamageStatics()
{
	static GJHStatusDamageStatics Statics;
	return Statics;
}

UGJHStatusDamageExecutionCalculation::UGJHStatusDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(StatusDamageStatics().MaxHealthDef);
	RelevantAttributesToCapture.Add(StatusDamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(StatusDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(StatusDamageStatics().StatusDamageRatioDef);
	RelevantAttributesToCapture.Add(StatusDamageStatics().StatusDamageResistDef);
}

void UGJHStatusDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	const float FinalDamage = CalcStatusDamage(ExecutionParams, EvaluationParameters, EffectSpec);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UGJHCharacterAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, FinalDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

float UGJHStatusDamageExecutionCalculation::CalcStatusDamage(const FGameplayEffectCustomExecutionParameters& InExecutionParams, const FAggregatorEvaluateParameters& InEvaluationParameters, const FGameplayEffectSpec& InEffectSpec) const
{
	float FinalDamage = 0.f;
	
#define CapturedStat(Var)	\
	float Var = 0.f;			\
	InExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StatusDamageStatics().Var##Def, InEvaluationParameters, Var);
	
	CapturedStat(MaxHealth)
	CapturedStat(Strength)
	CapturedStat(Armor)
	CapturedStat(StatusDamageRatio)
	CapturedStat(StatusDamageResist)
#undef CapturedStat
	
	const int32 StackCount = InEffectSpec.GetStackCount();
	FGameplayTagContainer SpecTags;
	InEffectSpec.GetAllGrantedTags(SpecTags);
	
	if (SpecTags.HasTagExact(FGJHGameplayTag::Status_Type_Poison_Deadly()))
	{
		const float BasePoison = (MaxHealth * 0.02f + Strength);
		FinalDamage = (BasePoison * (StatusDamageRatio / StatusDamageResist));
	}
	else if (SpecTags.HasTagExact(FGJHGameplayTag::Status_Type_Poison()))
	{
		const float BasePoison = (Strength * 0.5f) * StackCount;
		FinalDamage = (BasePoison * (StatusDamageRatio / StatusDamageResist)) - (Armor * 0.1f);
	}
	
	return FinalDamage;
}
