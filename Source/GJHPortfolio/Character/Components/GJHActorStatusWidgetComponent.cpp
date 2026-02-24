#include "GJHActorStatusWidgetComponent.h"

UGJHActorStatusWidgetComponent::UGJHActorStatusWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGJHActorStatusWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetNetMode() == NM_DedicatedServer)
		SetComponentTickEnabled(false);
}
