#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GJHBTTask_SendInputToAbilitySystem.generated.h"

class UGJHAbilitySystemComponent;

UCLASS()
class GJHPORTFOLIO_API UGJHBTTask_SendInputToAbilitySystem : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "GJH", meta = (Categories = "Ability.Skill"))
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, Category = "GJH")
	float AbortDistance = -1.f;

	UPROPERTY()
	TObjectPtr<UGJHAbilitySystemComponent> OwningAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> OwningBehaviorTreeComponent;

	FDelegateHandle AbilityEndedDelegateHandle;
	
public:
	UGJHBTTask_SendInputToAbilitySystem();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

private:
	void OnGameplayAbilityEnded(const FAbilityEndedData& EndedData);
};
