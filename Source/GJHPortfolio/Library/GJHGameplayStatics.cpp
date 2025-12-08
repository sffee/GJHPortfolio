#include "GJHGameplayStatics.h"

#include "GJHDataStatics.h"
#include "Character/Monster/GJHMonsterCharacter.h"
#include "Character/Player/GJHPlayerCharacter.h"
#include "Character/Player/GJHPlayerController.h"
#include "Character/Player/GJHPlayerState.h"
#include "Kismet/GameplayStatics.h"

AGJHPlayerCharacter* UGJHGameplayStatics::GetGJHPlayerCharacter(const UObject* InWorldContextObject)
{
	return Cast<AGJHPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(InWorldContextObject, 0));
}

AGJHPlayerController* UGJHGameplayStatics::GetGJHPlayerController(const UObject* InWorldContextObject)
{
	return Cast<AGJHPlayerController>(UGameplayStatics::GetPlayerController(InWorldContextObject, 0));
}

AGJHPlayerState* UGJHGameplayStatics::GetGJHPlayerState(const UObject* InWorldContextObject)
{
	AGJHPlayerController* PlayerController = GetGJHPlayerController(InWorldContextObject);
	return IsValid(PlayerController) ? PlayerController->GetPlayerState<AGJHPlayerState>() : nullptr;
}

FGameplayTag UGJHGameplayStatics::GetCurrentCharacterTypeTag(const UObject* InWorldContextObject)
{
	AGJHPlayerCharacter* PlayerCharacter = GetGJHPlayerCharacter(InWorldContextObject);
	return IsValid(PlayerCharacter) ? PlayerCharacter->GetCharacterTypeTag() : FGameplayTag::EmptyTag;
}

int32 UGJHGameplayStatics::GetCharacterLevel(AActor* InActor)
{
	AGJHCharacterBase* Character = Cast<AGJHCharacterBase>(InActor);
	return IsValid(Character) ? Character->GetCharacterLevel() : 0;
}

int32 UGJHGameplayStatics::GetPlayerCharacterLevel(UObject* InWorldContextObject)
{
	return GetCharacterLevel(UGameplayStatics::GetPlayerCharacter(InWorldContextObject, 0));
}

int32 UGJHGameplayStatics::GetMonsterIndex(AActor* InActor)
{
	AGJHMonsterCharacter* MonsterCharacter = Cast<AGJHMonsterCharacter>(InActor);
	return IsValid(MonsterCharacter) ? MonsterCharacter->GetMonsterIndex() : 0;
}

void UGJHGameplayStatics::AddXP(const UObject* InWorldContextObject, const int32 InAddXP)
{
	AGJHPlayerState* PlayerState = GetGJHPlayerState(InWorldContextObject);
	if (IsValid(PlayerState))
		PlayerState->AddXP(InAddXP);
}

void UGJHGameplayStatics::AddSkillPoint(const UObject* InWorldContextObject, const int32 InAddPoint)
{
	AGJHPlayerState* PlayerState = GetGJHPlayerState(InWorldContextObject);
	if (IsValid(PlayerState))
		PlayerState->AddSkillPoint(InAddPoint);
}

int32 UGJHGameplayStatics::GetSkillPoint(const UObject* InWorldContextObject)
{
	AGJHPlayerState* PlayerState = GetGJHPlayerState(InWorldContextObject);
	if (IsValid(PlayerState))
		return PlayerState->GetSkillPoint();

	return 0;
}
