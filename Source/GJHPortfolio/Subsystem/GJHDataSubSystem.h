#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/DataTable/GJHDataTableTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GJHDataSubSystem.generated.h"

class UGameplayAbility;

UCLASS()
class GJHPORTFOLIO_API UGJHDataSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	TMap<FGameplayTag, FGJHCharacterTableInfo> CharacterInfoMap;
	TMap<int32, FGJHMonsterTableInfo> MonsterInfoMap;
	TMap<FGameplayTag, FGJHMonsterRewardTableInfo> MonsterRewardInfoMap;
	TMap<FGameplayTag, TMap<int32, FGJHSkillTableInfo>> SkillInfoMap;
	TMap<int32, FGJHItemTableInfo> ItemInfoMap;
	
#if WITH_EDITORONLY_DATA
	static TMap<FGameplayTag, TSet<FGJHTableMemo>> TableMemoMap;
#endif
	
public:
	static UGJHDataSubSystem* Get(const UObject* WorldContextObject);
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	void InitCharacterInfoTable();
	void InitMonsterInfoTable();
	void InitMonsterRewardInfoTable();
	void InitSkillInfoTable();
	void InitItemInfoTable();

public:
	FGJHMonsterTableInfo GetMonsterInfo(int32 InMonsterIndex);
	FGJHMonsterRewardTableInfo GetMonsterRewardInfo(const FGameplayTag& InGradeTag);

public:
	FGJHSkillTableInfo GetSkillInfo(const FGameplayTag& InCharacterTypeTag, int32 InSkillIndex);
	int32 GetSkillIndex(const FGameplayTag& InCharacterTypeTag, const UGameplayAbility* InAbility);
	void ForeachSkillInfo(FGameplayTag InCharacterTypeTag, TFunctionRef<void(const FGJHSkillTableInfo&)> InFunc);

public:
	FGJHItemTableInfo GetItemInfo(int32 InItemIndex);
	
#if WITH_EDITOR
	static void GetTableMemo(const FGameplayTag& InTableTag, TSet<FGJHTableMemo>& OutMemos);
#endif
};
