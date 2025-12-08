#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GJHAbilitySystemStatics.generated.h"

class UGJHAbilitySystemComponent;
struct FGameplayEffectContextHandle;

UCLASS()
class GJHPORTFOLIO_API UGJHAbilitySystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UGJHAbilitySystemComponent* GetAbilitySystemComponent(AActor* InActor);
	
public:
	static void SetKnockBackImpulse(FGameplayEffectContextHandle& InEffectContextHandle, const FVector InKnockBackImpulse);
	static void SetDeathImpulse(FGameplayEffectContextHandle& InEffectContextHandle, const FVector InDeathImpulse);

public:
	
};
