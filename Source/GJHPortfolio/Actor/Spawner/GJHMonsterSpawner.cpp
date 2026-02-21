#include "GJHMonsterSpawner.h"

AGJHMonsterSpawner::AGJHMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGJHMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGJHMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
