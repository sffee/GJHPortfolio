#include "GJHAbilitySystemStatics.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GJHGameplayStatics.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "AbilitySystem/GJHAbilityTypes.h"

UGJHAbilitySystemComponent* UGJHAbilitySystemStatics::GetAbilitySystemComponent(AActor* InActor)
{
	return Cast<UGJHAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UGJHAbilitySystemStatics::SetKnockBackImpulse(FGameplayEffectContextHandle& InEffectContextHandle, const FVector InKnockBackImpulse)
{
	if (FGJHGameplayEffectContext* GJHEffectContext = static_cast<FGJHGameplayEffectContext*>(InEffectContextHandle.Get()))
		GJHEffectContext->SetKnockBackImpulse(InKnockBackImpulse);
}

void UGJHAbilitySystemStatics::SetDeathImpulse(FGameplayEffectContextHandle& InEffectContextHandle, const FVector InDeathImpulse)
{
	if (FGJHGameplayEffectContext* GJHEffectContext = static_cast<FGJHGameplayEffectContext*>(InEffectContextHandle.Get()))
		GJHEffectContext->SetDeathImpulse(InDeathImpulse);
}
