#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GJHAbilityTypes.h"

#include "GJHCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class AGJHCharacterBase;

USTRUCT()
struct FGJHEffectProperties
{
	GENERATED_BODY()

public:
	FGameplayEffectContextHandle EffectContextHandle;
	FGJHGameplayEffectContext* GJHGameplayEffectContext;

public:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC;

	UPROPERTY()
	TObjectPtr<AActor> SourceAvatarActor;

	UPROPERTY()
	TObjectPtr<AController> SourceController;

	UPROPERTY()
	TObjectPtr<AGJHCharacterBase> SourceCharacter;

public:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC;

	UPROPERTY()
	TObjectPtr<AActor> TargetAvatarActor;

	UPROPERTY()
	TObjectPtr<AController> TargetController;

	UPROPERTY()
	TObjectPtr<AGJHCharacterBase> TargetCharacter;
};

UCLASS()
class GJHPORTFOLIO_API UGJHCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& InData, FGJHEffectProperties& OutProperties);
	
private:
	void HandleIncomingDamage(const FGameplayEffectModCallbackData& Data, const FGJHEffectProperties& InGJHEffectProperties);
	void ProcessAddXP(const FGJHEffectProperties& InGJHEffectProperties) const;

private:
	void SendHitReactionEvent(const FGJHEffectProperties& InGJHEffectProperties) const;
	void SendDeathEvent(const FGJHEffectProperties& InGJHEffectProperties) const;

	void CreateDamageText(float InDamage, const FGJHEffectProperties& InGJHEffectProperties);
	
public:
	UPROPERTY()
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UGJHCharacterAttributeSet, IncomingDamage)
	
public:
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGJHCharacterAttributeSet, Health)

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGJHCharacterAttributeSet, MaxHealth)

	UPROPERTY(ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGJHCharacterAttributeSet, Mana)

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGJHCharacterAttributeSet, MaxMana)
	
private:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};
