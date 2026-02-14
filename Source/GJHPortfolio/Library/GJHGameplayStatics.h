#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GJHGameplayStatics.generated.h"

class AGJHCharacterBase;
class AGJHPlayerController;
class AGJHPlayerState;
class AGJHPlayerCharacter;

UCLASS()
class GJHPORTFOLIO_API UGJHGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static AGJHPlayerCharacter* GetGJHPlayerCharacter(const UObject* InWorldContextObject);
	static AGJHPlayerController* GetGJHPlayerController(const UObject* InWorldContextObject);
	static AGJHPlayerState* GetGJHPlayerState(const UObject* InWorldContextObject);
	static FGameplayTag GetCurrentCharacterTypeTag(const UObject* InWorldContextObject);

public:
	static int32 GetCharacterLevel(AActor* InActor);
	static int32 GetPlayerCharacterLevel(UObject* InWorldContextObject);
	
	static int32 GetMonsterIndex(AActor* InActor);
	
public:
	static void AddXP(const UObject* InWorldContextObject, const int32 InAddXP);
	static void AddSkillPoint(const UObject* InWorldContextObject, const int32 InAddPoint);

public:
	static int32 GetSkillPoint(const UObject* InWorldContextObject);
};
