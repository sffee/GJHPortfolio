#include "GJHLevelUpInfoDataAsset.h"

int32 UGJHLevelUpInfoDataAsset::FindLevelForXP(int32 InXP)
{
	int32 Level = 1;
	while (true)
	{
		if (LevelUpInfoData.Num() <= Level)
			break;

		if (LevelUpInfoData[Level].LevelUpRequire <= InXP)
			++Level;
		else
			break;
	}

	return Level;
}

int32 UGJHLevelUpInfoDataAsset::GetLevelUpRequire(int32 InLevel)
{
	if (LevelUpInfoData.IsValidIndex(InLevel))
		return LevelUpInfoData[InLevel].LevelUpRequire;

	return 0;
}

int32 UGJHLevelUpInfoDataAsset::GetAttributePointReward(int32 InLevel)
{
	if (LevelUpInfoData.IsValidIndex(InLevel))
		return LevelUpInfoData[InLevel].AttributePointReward;

	return 0;
}

int32 UGJHLevelUpInfoDataAsset::GetSkillPointPointReward(int32 InLevel)
{
	if (LevelUpInfoData.IsValidIndex(InLevel))
		return LevelUpInfoData[InLevel].SkillPointReward;

	return 0;
}
