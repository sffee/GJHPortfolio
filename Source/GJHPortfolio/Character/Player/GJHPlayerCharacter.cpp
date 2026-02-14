#include "GJHPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "EngineUtils.h"
#include "GJHPlayerState.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "AI/GJHAITypes.h"
#include "Camera/CameraComponent.h"
#include "Character/Monster/GJHMonsterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AGJHPlayerCharacter::AGJHPlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(GetRootComponent());
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AGJHPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGJHPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGJHPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystem();	
}

void AGJHPlayerCharacter::InitAbilitySystem()
{
	AGJHPlayerState* GJHPlayerState = GetPlayerState<AGJHPlayerState>();
	if (IsValid(GJHPlayerState) == false)
		return;

	CachedAbilitySystemComponent = Cast<UGJHAbilitySystemComponent>(GJHPlayerState->GetAbilitySystemComponent());
	if (CachedAbilitySystemComponent.IsValid() == false)
		return;

	CachedAbilitySystemComponent->InitAbilityActorInfo(GJHPlayerState, this);

	if (HasAuthority() && CachedAbilitySystemComponent.IsValid())
	{
		CachedAbilitySystemComponent->AddAbilities(StartUpAbilities);
		CachedAbilitySystemComponent->ApplyGameplayEffects(StartUpEffects);
	}
}

FGenericTeamId AGJHPlayerCharacter::GetGenericTeamId() const
{
	return static_cast<uint8>(EGJHTeamID::Player);
}

UAbilitySystemComponent* AGJHPlayerCharacter::GetAbilitySystemComponent() const
{
	const AGJHPlayerState* GJHPlayerState = GetPlayerState<AGJHPlayerState>();
	return IsValid(GJHPlayerState) ? GJHPlayerState->GetAbilitySystemComponent() : nullptr;
}
