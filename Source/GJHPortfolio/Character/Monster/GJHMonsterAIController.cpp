#include "GJHMonsterAIController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GJHMonsterCharacter.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "AI/GJHAITypes.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameplayTag/GJHGameplayTag.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AGJHMonsterAIController::AGJHMonsterAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->PeripheralVisionAngleDegrees = 180.f;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AGJHMonsterAIController::OnTargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AGJHMonsterAIController::OnTargetPerceptionForgotten);
}

void AGJHMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (OwningAbilitySystemComponent.IsValid())
	{
		HitReactionTagHandle = OwningAbilitySystemComponent->RegisterGameplayTagEvent(FGJHGameplayTag::Ability_Common_HitReaction()).AddUObject(this, &ThisClass::OnHitReactionTagUpdated);
		DeathTagHandle = OwningAbilitySystemComponent->RegisterGameplayTagEvent(FGJHGameplayTag::Ability_Common_Death()).AddUObject(this, &ThisClass::OnDeathReactionTagUpdated);
	}
}

void AGJHMonsterAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (OwningAbilitySystemComponent.IsValid())
	{
		OwningAbilitySystemComponent->RegisterGameplayTagEvent(FGJHGameplayTag::Ability_Common_HitReaction()).Remove(HitReactionTagHandle);
		OwningAbilitySystemComponent->RegisterGameplayTagEvent(FGJHGameplayTag::Ability_Common_Death()).Remove(DeathTagHandle);
	}
}

void AGJHMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwningMonsterCharacter = Cast<AGJHMonsterCharacter>(InPawn);
	OwningAbilitySystemComponent = Cast<UGJHAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InPawn));

	EnableBehaviorTree();
}

void AGJHMonsterAIController::EnableBehaviorTree()
{
	if (OwningMonsterCharacter.IsValid() == false)
		return;

	Blackboard->SetValueAsFloat(FGJHBlackboardKeyName::TargetDistance, FLT_MAX);
	RunBehaviorTree(OwningMonsterCharacter->GetBehaviorTreeAsset());
}

void AGJHMonsterAIController::DisableBehaviorTree()
{
	CleanupBrainComponent();
}

void AGJHMonsterAIController::SetTarget(AActor* InTargetActor)
{
	if (IsValid(InTargetActor))
		GetBlackboardComponent()->SetValueAsObject(FGJHBlackboardKeyName::Target, InTargetActor);
	else
		GetBlackboardComponent()->ClearValue(FGJHBlackboardKeyName::Target);
}

void AGJHMonsterAIController::OnHitReactionTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (0 < NewCount)
		DisableBehaviorTree();
	else if (OwningMonsterCharacter->IsDead() == false)
		EnableBehaviorTree();
}

void AGJHMonsterAIController::OnDeathReactionTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	DisableBehaviorTree();
	GetBlackboardComponent()->SetValueAsBool(FGJHBlackboardKeyName::IsDead, true);
}

void AGJHMonsterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	SetTarget(Actor);
}

void AGJHMonsterAIController::OnTargetPerceptionForgotten(AActor* Actor)
{
}

