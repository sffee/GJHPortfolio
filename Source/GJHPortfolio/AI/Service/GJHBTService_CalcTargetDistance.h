#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GJHBTService_CalcTargetDistance.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHBTService_CalcTargetDistance : public UBTService
{
	GENERATED_BODY()

public:
	UGJHBTService_CalcTargetDistance();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
