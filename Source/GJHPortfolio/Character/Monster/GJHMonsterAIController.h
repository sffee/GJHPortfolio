#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h"
#include "Perception/AIPerceptionTypes.h"

#include "GJHMonsterAIController.generated.h"

class AGJHMonsterCharacter;
class UGJHAbilitySystemComponent;
class UAISenseConfig_Sight;
class UBehaviorTreeComponent;

UCLASS()
class GJHPORTFOLIO_API AGJHMonsterAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

private:
	TWeakObjectPtr<AGJHMonsterCharacter> OwningMonsterCharacter;
	TWeakObjectPtr<UGJHAbilitySystemComponent> OwningAbilitySystemComponent;
	
	FDelegateHandle HitReactionTagHandle;
	FDelegateHandle DeathTagHandle;
	
public:
	AGJHMonsterAIController();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	void EnableBehaviorTree();
	void DisableBehaviorTree();

private:
	void SetTarget(AActor* InTargetActor);

public:
	UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return BehaviorTreeComponent; };

private:
	void OnHitReactionTagUpdated(const FGameplayTag Tag, int32 NewCount);
	void OnDeathReactionTagUpdated(const FGameplayTag Tag, int32 NewCount);
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnTargetPerceptionForgotten(AActor* Actor);
};
