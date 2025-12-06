#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Character/GJHCharacterBase.h"

#include "GJHPlayerCharacter.generated.h"

class UGameplayAbility;
class UGJHAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class GJHPORTFOLIO_API AGJHPlayerCharacter : public AGJHCharacterBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

private:
	UPROPERTY(EditAnywhere, Category = "GJH|Character", meta = (Categories = "Character.Type"))
	FGameplayTag CharacterTypeTag;

private:
	TWeakObjectPtr<UGJHAbilitySystemComponent> CachedAbilitySystemComponent;
	
public:
	AGJHPlayerCharacter();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

private:
	void InitAbilitySystem();

public:
	virtual FGenericTeamId GetGenericTeamId() const override;
	FORCEINLINE FGameplayTag GetCharacterTypeTag() const { return CharacterTypeTag; }
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
