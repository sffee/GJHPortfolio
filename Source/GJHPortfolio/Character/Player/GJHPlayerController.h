#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"

#include "GJHPlayerController.generated.h"

class UGJHInventoryComponent;
class UGJHDamageTextComponent;
class UGJHAbilitySystemComponent;
class UInputMappingContext;
class UGJHInputConfigDataAsset;

UCLASS()
class GJHPORTFOLIO_API AGJHPlayerController : public APlayerController, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UGJHInventoryComponent> InventoryComponent;
	
private:
	UPROPERTY(EditAnywhere, Category = "GJH|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "GJH|Input")
	TObjectPtr<UGJHInputConfigDataAsset> InputConfig;

	UPROPERTY(EditAnywhere, Category = "GJH|Widget")
	TSubclassOf<UGJHDamageTextComponent> DamageTextComponentClass;

public:
	AGJHPlayerController();
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void InitHUDWidget() const;
	
private:
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);

private:
	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);
	void Input_SkillTree(const FInputActionValue& Value);
	void Input_Inventory(const FInputActionValue& Value);

public:
	UFUNCTION(Client, Reliable)
	void Client_CreateDamageText(AActor* InTargetActor, float InDamage) const;
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UGJHInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
};
