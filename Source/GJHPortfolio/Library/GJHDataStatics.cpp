#include "GJHDataStatics.h"

#include "GJHGameplayStatics.h"
#include "AbilitySystem/GJHGameplayAbilityBase.h"
#include "GJHSettings/GJHDataDeveloperSettings.h"
#include "Item/Definition/GJHItemDefinition.h"
#include "Subsystem/GJHDataSubSystem.h"

UDataTable* UGJHDataStatics::GetDataTable(const FGameplayTag& InDataTableTypeTag)
{
	const UGJHDataDeveloperSettings* DataDeveloperSettings = GetDefault<UGJHDataDeveloperSettings>();

	for (const auto& [TypeTag, DataTable] : DataDeveloperSettings->DataTableMap)
	{
		if (TypeTag == InDataTableTypeTag)
			return DataTable.LoadSynchronous();
	}

	return nullptr;
}

FGJHMonsterTableInfo UGJHDataStatics::GetMonsterInfo(const UObject* InWorldContextObject, int32 InMonsterIndex)
{
	UGJHDataSubSystem* DataSubSystem = UGJHDataSubSystem::Get(InWorldContextObject);
	return IsValid(DataSubSystem) ? DataSubSystem->GetMonsterInfo(InMonsterIndex) : FGJHMonsterTableInfo();
}

FGameplayTag UGJHDataStatics::GetMonsterGrade(const UObject* InWorldContextObject, int32 InMonsterIndex)
{
	FGJHMonsterTableInfo MonsterInfo = GetMonsterInfo(InWorldContextObject, InMonsterIndex);
	return MonsterInfo.Grade;
}

float UGJHDataStatics::GetMonsterXP(const UObject* InWorldContextObject, int32 InMonsterIndex, int32 InMonsterLevel)
{
	if (InMonsterLevel <= 0)
		return 0.f;
	
	FGameplayTag Grade = GetMonsterGrade(InWorldContextObject, InMonsterIndex);
	if (Grade.IsValid() == false)
		return 0.f;

	UGJHDataSubSystem* DataSubSystem = UGJHDataSubSystem::Get(InWorldContextObject);
	if (IsValid(DataSubSystem) == false)
		return 0.f;
	
	FGJHMonsterRewardTableInfo MonsterRewardTableInfo = DataSubSystem->GetMonsterRewardInfo(Grade);
	return MonsterRewardTableInfo.RewardXP.GetValueAtLevel(InMonsterLevel);
}

FGJHSkillTableInfo UGJHDataStatics::GetSkillInfo(const UObject* InWorldContextObject, int32 InSkillIndex)
{
	const FGameplayTag CurrentCharacterTypeTag = UGJHGameplayStatics::GetCurrentCharacterTypeTag(InWorldContextObject);
	if (CurrentCharacterTypeTag.IsValid() == false)
		return FGJHSkillTableInfo();

	return GetSkillInfo(InWorldContextObject, CurrentCharacterTypeTag, InSkillIndex);
}

FGJHSkillTableInfo UGJHDataStatics::GetSkillInfo(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, int32 InSkillIndex)
{
	UGJHDataSubSystem* DataSubSystem = UGJHDataSubSystem::Get(InWorldContextObject);
	return IsValid(DataSubSystem) ? DataSubSystem->GetSkillInfo(InCharacterTypeTag, InSkillIndex) : FGJHSkillTableInfo();
}

int32 UGJHDataStatics::GetSkillIndex(const UObject* InWorldContextObject, const UGameplayAbility* InAbility)
{
	if (IsValid(InAbility) == false)
		return INVALID_SKILL_INDEX;
	
	const FGameplayTag CurrentCharacterTypeTag = UGJHGameplayStatics::GetCurrentCharacterTypeTag(InWorldContextObject);
	if (CurrentCharacterTypeTag.IsValid() == false)
		return INVALID_SKILL_INDEX;

	return GetSkillIndex(InWorldContextObject, CurrentCharacterTypeTag, InAbility);
}

int32 UGJHDataStatics::GetSkillIndex(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, const UGameplayAbility* InAbility)
{
	if (IsValid(InAbility) == false)
		return INVALID_SKILL_INDEX;
	
	UGJHDataSubSystem* DataSubSystem = UGJHDataSubSystem::Get(InWorldContextObject);
	return IsValid(DataSubSystem) ? DataSubSystem->GetSkillIndex(InCharacterTypeTag, InAbility) : INVALID_SKILL_INDEX;
}

UTexture2D* UGJHDataStatics::GetSkillIcon(const UObject* InWorldContextObject, int32 InSkillIndex)
{
	const FGameplayTag CurrentCharacterTypeTag = UGJHGameplayStatics::GetCurrentCharacterTypeTag(InWorldContextObject);
	if (CurrentCharacterTypeTag.IsValid() == false)
		return nullptr;

	return GetSkillIcon(InWorldContextObject, CurrentCharacterTypeTag, InSkillIndex);
}

UTexture2D* UGJHDataStatics::GetSkillIcon(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, int32 InSkillIndex)
{
	FGJHSkillTableInfo SkillInfo = GetSkillInfo(InWorldContextObject, InCharacterTypeTag, InSkillIndex);
	return SkillInfo.Icon.LoadSynchronous();
}

FGameplayTag UGJHDataStatics::GetSkillCooldownTag(const UObject* InWorldContextObject, const int32 InSkillIndex)
{
	const FGameplayTag CurrentCharacterTypeTag = UGJHGameplayStatics::GetCurrentCharacterTypeTag(InWorldContextObject);
	if (CurrentCharacterTypeTag.IsValid() == false)
		return FGameplayTag::EmptyTag;
	
	return GetSkillCooldownTag(InWorldContextObject, CurrentCharacterTypeTag, InSkillIndex);
}

FGameplayTag UGJHDataStatics::GetSkillCooldownTag(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, const int32 InSkillIndex)
{
	FGJHSkillTableInfo SkillInfo = GetSkillInfo(InWorldContextObject, InCharacterTypeTag, InSkillIndex);
	return SkillInfo.CooldownTag;
}

int32 UGJHDataStatics::GetSkillLevelRequireBySkillIndex(const UObject* InWorldContextObject, const int32 InSkillIndex)
{
	const FGameplayTag CurrentCharacterTypeTag = UGJHGameplayStatics::GetCurrentCharacterTypeTag(InWorldContextObject);
	if (CurrentCharacterTypeTag.IsValid() == false)
		return -1;

	return GetSkillLevelRequireBySkillIndex(InWorldContextObject, CurrentCharacterTypeTag, InSkillIndex);
}

int32 UGJHDataStatics::GetSkillLevelRequireBySkillIndex(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, const int32 InSkillIndex)
{
	FGJHSkillTableInfo SkillInfo = GetSkillInfo(InWorldContextObject, InCharacterTypeTag, InSkillIndex);
	return SkillInfo.LevelRequirement;
}

void UGJHDataStatics::ForeachSkillInfo(const UObject* InWorldContextObject, TFunctionRef<void(const FGJHSkillTableInfo&)> InFunc)
{
	FGameplayTag CurrentCharacterTypeTag = UGJHGameplayStatics::GetCurrentCharacterTypeTag(InWorldContextObject);
	if (CurrentCharacterTypeTag.IsValid() == false)
		return;

	ForeachSkillInfo(InWorldContextObject, CurrentCharacterTypeTag, MoveTemp(InFunc));
}

void UGJHDataStatics::ForeachSkillInfo(const UObject* InWorldContextObject, const FGameplayTag& InCharacterTypeTag, TFunctionRef<void(const FGJHSkillTableInfo&)> InFunc)
{
	UGJHDataSubSystem* DataSubSystem = UGJHDataSubSystem::Get(InWorldContextObject);
	if (IsValid(DataSubSystem))
	{
		DataSubSystem->ForeachSkillInfo(InCharacterTypeTag, MoveTemp(InFunc));
	}
}

TSubclassOf<UGJHItemDefinition> UGJHDataStatics::GetItemDefinition(const UObject* InWorldContextObject, const int32 InItemIndex)
{
	UGJHDataSubSystem* DataSubSystem = UGJHDataSubSystem::Get(InWorldContextObject);
	return IsValid(DataSubSystem) ? DataSubSystem->GetItemInfo(InItemIndex).ItemDefinition : nullptr;
}
