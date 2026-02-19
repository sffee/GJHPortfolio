#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"

#include "GJHAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FGJHGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

private:
	FVector KnockBackImpulse = FVector::ZeroVector;
	FVector DeathImpulse = FVector::ZeroVector;
	
public:
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const
	{
		FGJHGameplayEffectContext* NewContext = new FGJHGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

public:
	FORCEINLINE void SetKnockBackImpulse(const FVector& InKnockBackImpulse) { KnockBackImpulse = InKnockBackImpulse; }
	FORCEINLINE void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }

	FORCEINLINE FVector GetKnockBackImpulse() { return KnockBackImpulse; }
	FORCEINLINE FVector GetDeathImpulse() { return DeathImpulse; }
};

template<>
struct TStructOpsTypeTraits<FGJHGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FGJHGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

USTRUCT(BlueprintType)
struct FGJHDamageKindData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FScalableFloat Damage;

	UPROPERTY(EditAnywhere)
	float KnockBackImpulseX = 0.f;

	UPROPERTY(EditAnywhere)
	float KnockBackImpulseZ = 0.f;

	UPROPERTY(EditAnywhere)
	float DeathImpulseX = 0.f;

	UPROPERTY(EditAnywhere)
	float DeathImpulseZ = 0.f;
};

UCLASS()
class UGJHReactionInfo : public UObject
{
	GENERATED_BODY()

public:
	FVector KnockBackImpulse = FVector::ZeroVector;
};

UCLASS()
class UGJHDeathInfo : public UObject
{
	GENERATED_BODY()

public:
	FVector DeathImpulse = FVector::ZeroVector;
};
