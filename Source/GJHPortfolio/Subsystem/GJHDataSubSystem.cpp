#include "GJHDataSubSystem.h"

#include "AbilitySystem/GJHGameplayAbilityBase.h"
#include "GameplayTag/GJHGameplayTag.h"
#include "GJHSettings/GJHDataDeveloperSettings.h"
#include "Library/GJHDataStatics.h"

UGJHDataSubSystem* UGJHDataSubSystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UGJHDataSubSystem>(World->GetGameInstance());
	}

	return nullptr;
}

void UGJHDataSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InitCharacterInfoTable();
	InitMonsterInfoTable();
	InitMonsterRewardInfoTable();
	InitSkillInfoTable();
	InitItemInfoTable();
}

void UGJHDataSubSystem::InitCharacterInfoTable()
{
	UGJHDataStatics::ForeachDataTableInfo<FGJHCharacterTableInfo>(FGJHGameplayTag::DataTable_Type_CharacterTableInfo() , [this](const FGJHCharacterTableInfo& InCharacterInfo)
	{
		CharacterInfoMap.FindOrAdd(InCharacterInfo.CharacterTag) = InCharacterInfo;
	});
}

void UGJHDataSubSystem::InitMonsterInfoTable()
{
	UGJHDataStatics::ForeachDataTableInfo<FGJHMonsterTableInfo>(FGJHGameplayTag::DataTable_Type_MonsterTableInfo() , [this](const FGJHMonsterTableInfo& InMonsterInfo)
	{
		MonsterInfoMap.FindOrAdd(InMonsterInfo.Index) = InMonsterInfo;
	});
}

void UGJHDataSubSystem::InitMonsterRewardInfoTable()
{
	UGJHDataStatics::ForeachDataTableInfo<FGJHMonsterRewardTableInfo>(FGJHGameplayTag::DataTable_Type_MonsterRewardTableInfo() , [this](const FGJHMonsterRewardTableInfo& InMonsterRewardInfo)
	{
		MonsterRewardInfoMap.FindOrAdd(InMonsterRewardInfo.Grade) = InMonsterRewardInfo;
	});
}

void UGJHDataSubSystem::InitSkillInfoTable()
{
	for (const auto& [CharacterTag, CharacterInfo] : CharacterInfoMap)
	{
		if (CharacterInfo.SkillInfoTableTag.IsValid() == false)
			continue;

		UGJHDataStatics::ForeachDataTableInfo<FGJHSkillTableInfo>(CharacterInfo.SkillInfoTableTag, [this, CharacterTag](const FGJHSkillTableInfo& InSkillInfo)
		{
			SkillInfoMap.FindOrAdd(CharacterTag).FindOrAdd(InSkillInfo.Index) = InSkillInfo;
		});
	}
}

void UGJHDataSubSystem::InitItemInfoTable()
{
	UGJHDataStatics::ForeachDataTableInfo<FGJHItemTableInfo>(FGJHGameplayTag::DataTable_Type_ItemTableInfo() , [this](const FGJHItemTableInfo& InItemInfo)
	{
		ItemInfoMap.FindOrAdd(InItemInfo.Index) = InItemInfo;
	});
}

FGJHMonsterTableInfo UGJHDataSubSystem::GetMonsterInfo(int32 InMonsterIndex)
{
	if (MonsterInfoMap.Contains(InMonsterIndex) == false)
	{
		ensureMsgf(false, TEXT("Monster Info Not Found!!"));
		return FGJHMonsterTableInfo();
	}

	return MonsterInfoMap[InMonsterIndex];
}

FGJHMonsterRewardTableInfo UGJHDataSubSystem::GetMonsterRewardInfo(const FGameplayTag& InGradeTag)
{
	if (MonsterRewardInfoMap.Contains(InGradeTag))
		return MonsterRewardInfoMap[InGradeTag];

	return FGJHMonsterRewardTableInfo();
}

FGJHSkillTableInfo UGJHDataSubSystem::GetSkillInfo(const FGameplayTag& InCharacterTypeTag, int32 InSkillIndex)
{
	if (SkillInfoMap.Contains(InCharacterTypeTag) == false)
		return FGJHSkillTableInfo();

	if (SkillInfoMap[InCharacterTypeTag].Contains(InSkillIndex) == false)
		return FGJHSkillTableInfo();

	return SkillInfoMap[InCharacterTypeTag][InSkillIndex];
}

int32 UGJHDataSubSystem::GetSkillIndex(const FGameplayTag& InCharacterTypeTag, const UGameplayAbility* InAbility)
{
	if (SkillInfoMap.Contains(InCharacterTypeTag) == false)
		return INVALID_SKILL_INDEX;

	for (const auto& [SkillIndex, SkillTableInfo] : SkillInfoMap[InCharacterTypeTag])
	{
		if (SkillTableInfo.Ability && SkillTableInfo.Ability->GetClass() == InAbility->GetClass())
			return SkillIndex;
	}

	return INVALID_SKILL_INDEX;
}

void UGJHDataSubSystem::ForeachSkillInfo(FGameplayTag InCharacterTypeTag, TFunctionRef<void(const FGJHSkillTableInfo&)> InFunc)
{
	if (SkillInfoMap.Contains(InCharacterTypeTag) == false)
		return;

	for (auto Iter = SkillInfoMap[InCharacterTypeTag].CreateConstIterator(); Iter; ++Iter)
	{
		InFunc(Iter->Value);
	}
}

FGJHItemTableInfo UGJHDataSubSystem::GetItemInfo(int32 InItemIndex)
{
	if (ItemInfoMap.Contains(InItemIndex) == false)
		return FGJHItemTableInfo();

	return ItemInfoMap[InItemIndex];
}
