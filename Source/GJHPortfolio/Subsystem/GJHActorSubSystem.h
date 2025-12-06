#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GJHActorSubSystem.generated.h"

class AGJHMonsterCharacter;
class AGJHPlayerCharacter;
class AGJHCharacterBase;

UCLASS()
class GJHPORTFOLIO_API UGJHActorSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<AGJHPlayerCharacter> PlayerCharacter;

private:
	UPROPERTY()
	TSet<TObjectPtr<AGJHMonsterCharacter>> MonsterCharacterSet;

public:
	void RegisterPlayerCharacter(AGJHPlayerCharacter* InPlayerCharacter);
	void UnRegisterPlayerCharacter(AGJHPlayerCharacter* InPlayerCharacter);

	AGJHPlayerCharacter* GetPlayerCharacter() const;
	
};
