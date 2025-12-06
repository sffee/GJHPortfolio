#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"

#include "GJHCharacterBase.generated.h"

class UGameplayEffect;
class UGameplayAbility;

UCLASS()
class GJHPORTFOLIO_API AGJHCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "GJH|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;

	UPROPERTY(EditAnywhere, Category = "GJH|Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartUpEffects;

protected:
	int32 CharacterLevel = 1;
	
protected:
	bool bDeath = false;

	FDelegateHandle DeathTagDelegateHandle;
	
public:
	AGJHCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

public:
	void AddCharacterLevel(int32 InAddLevel);
	
public:
	virtual void OnDeath() { }

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return nullptr; }
	FORCEINLINE bool IsDead() const { return bDeath; }
	FORCEINLINE int32 GetCharacterLevel() const { return CharacterLevel; }
	
private:
	void OnDeathTagUpdated(const FGameplayTag Tag, int32 NewCount);
};
