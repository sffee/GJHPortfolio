#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "ScalableFloat.h"

#include "GJHDataTableTypes.generated.h"

#define INVALID_SKILL_INDEX -1
#define INVALID_ITEM_INDEX -1

class AGJHMonsterCharacter;
class UGJHItemDefinition;
class UGJHMonsterRewardDataAsset;
class UGJHGameplayAbilityBase;

USTRUCT(BlueprintType)
struct FGJHTableKey
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Table")
	int32 Key = -1;
	
public:
	FGJHTableKey()
	{ }
	
	FGJHTableKey(int32 InKey)
		: Key(InKey)
	{ }
};

#if WITH_EDITOR
USTRUCT(BlueprintType)
struct FGJHTableMemo
{
	GENERATED_BODY()
	
public:
	int32 Index = -1;
	FString Memo = FString();
	
public:
	FGJHTableMemo()
	{ }
	
	FGJHTableMemo(int32 InIndex)
		: Index(InIndex)
	{ }
	
	FGJHTableMemo(int32 InIndex, const FString& InMemo)
		: Index(InIndex), Memo(InMemo)
	{ }
	
	bool operator==(const FGJHTableMemo& Other) const
	{
		return Index == Other.Index;
	}
	
	friend uint32 GetTypeHash(const FGJHTableMemo& Other)
	{
		return GetTypeHash(Other.Index);
	}
	
public:
	bool IsValidKey() const { return Index != -1 && Memo.IsEmpty() == false; }
};
#endif

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

	UPROPERTY(EditAnywhere, meta = (GJHTableKey))
	int32 Index = 0;
	
	UPROPERTY(EditAnywhere, meta = (GJHTableMemo))
	FName MonsterName;

	UPROPERTY(EditAnywhere, meta = (Categories = "Character.Monster.Grade"))
	FGameplayTag Grade = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGJHMonsterCharacter> MonsterBP;
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
