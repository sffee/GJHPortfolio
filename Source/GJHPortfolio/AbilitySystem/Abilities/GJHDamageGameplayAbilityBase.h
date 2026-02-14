// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GJHAbilityTypes.h"
#include "AbilitySystem/GJHGameplayAbilityBase.h"
#include "GameplayTag/GJHGameplayTag.h"
#include "GJHDamageGameplayAbilityBase.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHDamageGameplayAbilityBase : public UGJHGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Ability|Default", meta = (ForceInlineRow))
	TMap<int32, FGJHDamageKindData> DamageKindData;
	
	UPROPERTY(EditAnywhere, Category = "Ability|Default")
	TSubclassOf<UGameplayEffect> DamageEffect;

protected:
	float GetDamage(int32 InComboIndex);
	void ApplyDamage(float InDamage, AActor* InTarget, const int32 InComboIndex, const FGameplayTag& InDamageType = FGJHGameplayTag::Ability_DamageType_Attack()) const;

private:
	FGameplayEffectContextHandle MakeDamageEffectContext(AActor* InTarget, const int32 InComboIndex) const;
};
