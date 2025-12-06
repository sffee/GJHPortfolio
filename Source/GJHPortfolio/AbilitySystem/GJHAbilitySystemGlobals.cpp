#include "GJHAbilitySystemGlobals.h"

#include "GJHAbilityTypes.h"

FGameplayEffectContext* UGJHAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FGJHGameplayEffectContext();
}
