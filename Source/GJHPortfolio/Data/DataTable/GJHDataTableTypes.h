#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Engine/DataTable.h"
#include "AttributeSet.h"

#include "GJHDataTableTypes.generated.h"

#define INVALID_SKILL_INDEX -1
#define INVALID_ITEM_INDEX -1

class UGJHItemDefinition;
class UGJHMonsterRewardDataAsset;
class UGJHGameplayAbilityBase;

USTRUCT()
struct FGJHCharacterTableInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "Character.Type"))
	FGameplayTag CharacterTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, meta = (Categories = "DataTable.Type.SkillTableInfo"))
	FGameplayTag SkillInfoTableTag = FGameplayTag::EmptyTag;
};

USTRUCT()
struct FGJHMonsterTableInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "Character.Type"))
	int32 Index = 0;

	UPROPERTY(EditAnywhere, meta = (Categories = "Character.Monster.Grade"))
	FGameplayTag Grade = FGameplayTag::EmptyTag;
};

USTRUCT()
struct FGJHSkillTableInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Index = INVALID_SKILL_INDEX;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditAnywhere)
	FString Name = FString();
	
	UPROPERTY(EditDefaultsOnly)
	int32 LevelRequirement = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 MinLevel = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxLevel = 1;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGJHGameplayAbilityBase> Ability;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Ability.Cooldown"))
	FGameplayTag CooldownTag;

public:
	bool IsValid() const
	{
		return Index != INVALID_SKILL_INDEX;
	}
};

USTRUCT()
struct FGJHMonsterRewardTableInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "Character.Monster.Grade"))
	FGameplayTag Grade = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, Category = "GJH")
	FScalableFloat RewardXP;
};

USTRUCT()
struct FGJHItemTableInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Index = INVALID_ITEM_INDEX;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGJHItemDefinition> ItemDefinition;

public:
	bool IsValid() const
	{
		return Index != INVALID_ITEM_INDEX;
	}
};