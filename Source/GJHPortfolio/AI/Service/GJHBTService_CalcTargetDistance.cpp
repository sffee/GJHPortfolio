#include "GJHBTService_CalcTargetDistance.h"

#include "AIController.h"
#include "AI/GJHAITypes.h"
#include "BehaviorTree/BlackboardComponent.h"

UGJHBTService_CalcTargetDistance::UGJHBTService_CalcTargetDistance()
{
	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UGJHBTService_CalcTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(FGJHBlackboardKeyName::Target));

	if (IsValid(TargetActor) == false)
		return;

	const AActor* OwningCharacter = OwnerComp.GetAIOwner()->GetPawn();
	const float Distance = FVector::Distance(TargetActor->GetActorLocation(), OwningCharacter->GetActorLocation());
	BlackboardComponent->SetValueAsFloat(FGJHBlackboardKeyName::TargetDistance, Distance);
}
