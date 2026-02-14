#include "GJHPlayerState.h"

#include "GJHPlayerCharacter.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "Data/DataAsset/GJHLevelUpInfoDataAsset.h"
#include "Library/GJHAbilitySystemStatics.h"
#include "Library/GJHGameplayStatics.h"

AGJHPlayerState::AGJHPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGJHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UGJHCharacterAttributeSet>(TEXT("AttributeSet"));
}

void AGJHPlayerState::AddXP(int32 InXP)
{
	XP += InXP;

	ProcessLevelUp();
	ProcessXPPercent();
}

void AGJHPlayerState::AddAttributePoint(int32 InPoint)
{
	AttributePoint += InPoint;
	OnChangeAttributePoint.Broadcast(AttributePoint);
}

void AGJHPlayerState::AddSkillPoint(int32 InPoint)
{
	if (SkillPoint + InPoint < 0)
		return;

	SkillPoint += InPoint;
	OnChangeSkillPoint.Broadcast(SkillPoint);
}

void AGJHPlayerState::ProcessLevelUp()
{
	if (IsValid(LevelUpInfo) == false)
	{
		ensureMsgf(false, TEXT("LevelUpInfo Not Set!!"));
		return;
	}
	
	AGJHPlayerCharacter* PlayerCharacter = UGJHGameplayStatics::GetGJHPlayerCharacter(this);
	if (IsValid(PlayerCharacter) == false)
		return;
	
	const int32 CurrentLevel = PlayerCharacter->GetCharacterLevel();

	const int32 NewLevel = LevelUpInfo->FindLevelForXP(XP);
	const int32 NumLevelUps = NewLevel - CurrentLevel;
	if (0 < NumLevelUps)
	{
		PlayerCharacter->AddCharacterLevel(NumLevelUps);

		int32 AttributePointReward = 0;
		int32 SkillPointsReward = 0;

		for (int32 i = 0; i < NumLevelUps; ++i)
		{
			AttributePointReward += LevelUpInfo->GetAttributePointReward(CurrentLevel + i);
			SkillPointsReward += LevelUpInfo->GetSkillPointPointReward(CurrentLevel + i);
		}

		if (0 < AttributePointReward)
			AddAttributePoint(AttributePointReward);

		if (0 < SkillPointsReward)
			AddSkillPoint(SkillPointsReward);

		OnLevelUp.Broadcast(NewLevel);
	}
}

void AGJHPlayerState::ProcessXPPercent()
{
	if (IsValid(LevelUpInfo) == false)
		return;

	const int32 Level = LevelUpInfo->FindLevelForXP(XP);
	const int32 MaxLevel = LevelUpInfo->GetMaxLevel();

	if (Level < MaxLevel)
	{
		const int32 LevelUpRequire = LevelUpInfo->GetLevelUpRequire(Level);
		const int32 PrevLevelUpRequire = LevelUpInfo->GetLevelUpRequire(Level - 1);

		const int32 DeltaLevelUpRequire = LevelUpRequire - PrevLevelUpRequire;

		const float XPPercent = static_cast<float>(XP - PrevLevelUpRequire) / static_cast<float>(DeltaLevelUpRequire);
		OnXPPercentChanged.Broadcast(XPPercent);
	}
	else
	{
		OnXPPercentChanged.Broadcast(100.f);
	}
}

UAbilitySystemComponent* AGJHPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
