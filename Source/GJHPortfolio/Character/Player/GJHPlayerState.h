#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "GameFramework/PlayerState.h"

#include "GJHPlayerState.generated.h"

class UGJHLevelUpInfoDataAsset;
class UGJHCharacterAttributeSet;
class UGJHAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FGJHOnXPPercentChanged, float NewPercent);
DECLARE_MULTICAST_DELEGATE_OneParam(FGJHOnLevelUp, int32 NewLevel);
DECLARE_MULTICAST_DELEGATE_OneParam(FGJHOnChangePoint, int32 NewPoint);

UCLASS()
class GJHPORTFOLIO_API AGJHPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<UGJHAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UGJHCharacterAttributeSet> AttributeSet;

private:
	UPROPERTY(EditAnywhere, Category = "InfoData")
	TObjectPtr<UGJHLevelUpInfoDataAsset> LevelUpInfo;

private:
	float XP = 0.f;
	int32 AttributePoint = 0;
	int32 SkillPoint = 0;

public:
	FGJHOnXPPercentChanged OnXPPercentChanged;
	FGJHOnLevelUp OnLevelUp;
	FGJHOnChangePoint OnChangeAttributePoint;
	FGJHOnChangePoint OnChangeSkillPoint;
	
public:
	AGJHPlayerState();

public:
	void AddXP(int32 InXP);
	void AddAttributePoint(int32 InPoint);
	void AddSkillPoint(int32 InPoint);

private:
	void ProcessLevelUp();
	void ProcessXPPercent();

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; } 
	FORCEINLINE float GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoint() const { return AttributePoint; }
	FORCEINLINE int32 GetSkillPoint() const { return SkillPoint; }
};
