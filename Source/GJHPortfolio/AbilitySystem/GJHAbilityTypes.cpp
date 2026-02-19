#include "GJHAbilityTypes.h"

bool FGJHGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	Super::NetSerialize(Ar, Map, bOutSuccess);
	
	const bool bIsSaving = Ar.IsSaving();
	
	SerializeOptionalValue(bIsSaving, Ar, KnockBackImpulse, FVector::ZeroVector);
	SerializeOptionalValue(bIsSaving, Ar, DeathImpulse, FVector::ZeroVector);
	
	bOutSuccess = true;
	return true;
}
