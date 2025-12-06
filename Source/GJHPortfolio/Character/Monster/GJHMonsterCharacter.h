#pragma once

#include "CoreMinimal.h"
#include "Character/GJHCharacterBase.h"

#include "GJHMonsterCharacter.generated.h"

class UGJHMonsterRewardDataAsset;
class UGJHOverHeadWidgetComponent;
class UBehaviorTree;
class UGJHCharacterAttributeSet;
class UGJHAbilitySystemComponent;

UCLASS()
class GJHPORTFOLIO_API AGJHMonsterCharacter : public AGJHCharacterBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<UGJHAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UGJHCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGJHOverHeadWidgetComponent> OverHeadWidgetComponent; 

private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	int32 MonsterIndex = -1;

	UPROPERTY(EditAnywhere, Category = "GJH")
	int32 MonsterLevel = 1;
	
	UPROPERTY(EditAnywhere, Category = "GJH|AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

private:
	
public:
	AGJHMonsterCharacter();

	virtual void BeginPlay() override;

public:
	virtual void OnDeath() override;

private:
	void InitAbilitySystem();
	void InitOverHeadWidgetComponent() const;

protected:
	virtual FGenericTeamId GetGenericTeamId() const override;

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTreeAsset() const { return BehaviorTreeAsset; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE int32 GetMonsterIndex() const { return MonsterIndex; }
	FORCEINLINE int32 GetMonsterLevel() const { return MonsterLevel; }
};
