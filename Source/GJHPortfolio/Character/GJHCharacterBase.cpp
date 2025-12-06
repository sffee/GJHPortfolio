#include "GJHCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTag/GJHGameplayTag.h"
#include "Subsystem/GJHActorSubSystem.h"

AGJHCharacterBase::AGJHCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
}

void AGJHCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GetAbilitySystemComponent()))
		DeathTagDelegateHandle = GetAbilitySystemComponent()->RegisterGameplayTagEvent(FGJHGameplayTag::Ability_Common_Death()).AddUObject(this, &ThisClass::OnDeathTagUpdated);
}

void AGJHCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(GetAbilitySystemComponent()))
		GetAbilitySystemComponent()->RegisterGameplayTagEvent(FGJHGameplayTag::Ability_Common_Death()).Remove(DeathTagDelegateHandle);
}

void AGJHCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGJHCharacterBase::AddCharacterLevel(int32 InAddLevel)
{
	CharacterLevel += InAddLevel;
}

void AGJHCharacterBase::OnDeathTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	OnDeath();
}
