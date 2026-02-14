#pragma once

UENUM(BlueprintType)
enum class EGJHTeamID : uint8
{
	Player = 0,
	Monster = 100
};

namespace FGJHBlackboardKeyName
{
	const FName Target = TEXT("Target");
	const FName TargetDistance = TEXT("TargetDistance");
	const FName IsDead = TEXT("IsDead");
}