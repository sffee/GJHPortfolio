#include "GJHMonsterCharacter.h"

#include "GJHMonsterAIController.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "AbilitySystem/Attribute/GJHCharacterAttributeSet.h"
#include "AI/GJHAITypes.h"
#include "Character/Components/GJHOverHeadWidgetComponent.h"
#include "UI/Status/GJHOverHeadStatusWidget.h"

AGJHMonsterCharacter::AGJHMonsterCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGJHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UGJHCharacterAttributeSet>(TEXT("AttributeSet"));
	
	OverHeadWidgetComponent = CreateDefaultSubobject<UGJHOverHeadWidgetComponent>(TEXT("OverHeadWidgetComponent"));
	OverHeadWidgetComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	OverHeadWidgetComponent->SetupAttachment(GetMesh());
}

void AGJHMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystem();
	InitOverHeadWidgetComponent();
}

void AGJHMonsterCharacter::OnDeath()
{
	Super::OnDeath();

	bDeath = true;
}

void AGJHMonsterCharacter::InitAbilitySystem()
{
	if (HasAuthority() == false)
		return;
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->AddAbilities(StartUpAbilities);
	AbilitySystemComponent->ApplyGameplayEffects(StartUpEffects);
}

void AGJHMonsterCharacter::InitOverHeadWidgetComponent() const
{
	UGJHOverHeadStatusWidget* OverHeadStatusWidget = Cast<UGJHOverHeadStatusWidget>(OverHeadWidgetComponent->GetUserWidgetObject());
	if (IsValid(OverHeadStatusWidget))
	{
		OverHeadStatusWidget->SetAbilitySystemComponent(AbilitySystemComponent);
	}
}

FGenericTeamId AGJHMonsterCharacter::GetGenericTeamId() const
{
	return static_cast<uint8>(EGJHTeamID::Monster);
}

UAbilitySystemComponent* AGJHMonsterCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
