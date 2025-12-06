#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GJHLevelUpInfoDataAsset.generated.h"

USTRUCT()
struct FGJHLevelUpInfoData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 LevelUpRequire = 0;

	UPROPERTY(EditAnywhere)
	int32 AttributePointReward = 0;

	UPROPERTY(EditAnywhere)
	int32 SkillPointReward = 0;
};

UCLASS()
class GJHPORTFOLIO_API UGJHLevelUpInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TArray<FGJHLevelUpInfoData> LevelUpInfoData;

public:
	int32 FindLevelForXP(int32 InXP);
	int32 GetLevelUpRequire(int32 InLevel);
	int32 GetAttributePointReward(int32 InLevel);
	int32 GetSkillPointPointReward(int32 InLevel);
	int32 GetMaxLevel() const { return LevelUpInfoData.Num(); }
};
