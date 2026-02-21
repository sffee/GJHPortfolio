#pragma once

#include "CoreMinimal.h"
#include "Data/DataTable/GJHDataTableTypes.h"
#include "GameFramework/Actor.h"
#include "GJHMonsterSpawner.generated.h"

UCLASS()
class GJHPORTFOLIO_API AGJHMonsterSpawner : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (DisplayName = "MonsterIndex", GJHTableTag = "DataTable.Type.MonsterTableInfo"))
	FGJHTableKey MonsterIndex = -1;
	
public:
	AGJHMonsterSpawner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
