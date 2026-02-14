#include "GJHBTTask_SendInputToAbilitySystem.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "AI/GJHAITypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Library/GJHAbilitySystemStatics.h"

UGJHBTTask_SendInputToAbilitySystem::UGJHBTTask_SendInputToAbilitySystem()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UGJHBTTask_SendInputToAbilitySystem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwningBehaviorTreeComponent = &OwnerComp;
	
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{
		OwningAbilitySystemComponent = UGJHAbilitySystemStatics::GetAbilitySystemComponent(AIC->GetPawn());
		if (OwningAbilitySystemComponent)
		{
			OwningAbilitySystemComponent->AbilityInputPressedByAbilityTag(AbilityTag);
			AbilityEndedDelegateHandle = OwningAbilitySystemComponent->OnAbilityEnded.AddUObject(this, &ThisClass::OnGameplayAbilityEnded);
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UGJHBTTask_SendInputToAbilitySystem::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IsValid(OwningAbilitySystemComponent))
	{
		OwningAbilitySystemComponent->CancelAbilityByAbilityTag(AbilityTag);
		OwningAbilitySystemComponent->OnAbilityEnded.Remove(AbilityEndedDelegateHandle);
		OwningAbilitySystemComponent = nullptr;
	}
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UGJHBTTask_SendInputToAbilitySystem::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (IsValid(BlackboardComponent) == false)
		return;

	const float TargetDistance = BlackboardComponent->GetValueAsFloat(FGJHBlackboardKeyName::TargetDistance);
	if (0.f < AbortDistance && AbortDistance < TargetDistance)
		return;

	if (IsValid(OwningAbilitySystemComponent))
		OwningAbilitySystemComponent->AbilityInputPressedByAbilityTag(AbilityTag);
}

FString UGJHBTTask_SendInputToAbilitySystem::GetStaticDescription() const
{
	return FString::Printf(TEXT("AbilityTag : %s"), *AbilityTag.ToString());
}

void UGJHBTTask_SendInputToAbilitySystem::OnGameplayAbilityEnded(const FAbilityEndedData& EndedData)
{
	if (IsValid(OwningAbilitySystemComponent) == false)
		return;
	
	FGameplayAbilitySpec* AbilitySpec = OwningAbilitySystemComponent->FindAbilitySpecFromHandle(EndedData.AbilitySpecHandle);
	if (AbilitySpec == nullptr)
		return;

	if (AbilitySpec->Ability->GetAssetTags().HasTagExact(AbilityTag))
		FinishLatentTask(*OwningBehaviorTreeComponent, EBTNodeResult::Succeeded);
}
