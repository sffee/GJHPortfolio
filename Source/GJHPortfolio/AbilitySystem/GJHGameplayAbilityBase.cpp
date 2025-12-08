#include "GJHGameplayAbilityBase.h"

#include "Attribute/GJHCharacterAttributeSet.h"
#include "Character/Player/GJHPlayerCharacter.h"

void UGJHGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CharacterType = Cast<AGJHPlayerCharacter>(ActorInfo->AvatarActor.Get()) ? EGJHCharacterType::Player : EGJHCharacterType::Monster;
}

void UGJHGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FString UGJHGameplayAbilityBase::GetDescription()
{
	FString Description;
	Description = FString::Printf(TEXT("<Yellow>Level</> : %d\n"), FMath::Max(GetAbilityLevel(), 1));

	if (GetCostGameplayEffect())
		Description += FString::Printf(TEXT("<Blue>MP 소모량</> : %.1f\n"), FMath::Abs(GetManaCost()));

	if (GetCooldownGameplayEffect())
		Description += FString::Printf(TEXT("<Cyan>Cooldown</> : %.1f\n"), FMath::Max(GetCooldown(), 0.f));

	Description += FString(TEXT("=============================\n"));

	return Description;
}

float UGJHGameplayAbilityBase::GetManaCost()
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UGJHCharacterAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(FMath::Max(GetAbilityLevel(), 1), ManaCost);
				break;
			}
		}
	}

	return ManaCost;
}

float UGJHGameplayAbilityBase::GetCooldown()
{
	float Cooldown = 0.f;
	if (GetCooldownGameplayEffect())
		GetCooldownGameplayEffect()->DurationMagnitude.GetStaticMagnitudeIfPossible(GetAbilityLevel(), Cooldown);

	return Cooldown;
}
