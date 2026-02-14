#include "GJHPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "GJHPlayerState.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "Character/Components/GJHDamageTextComponent.h"
#include "Components/GJHInputComponent.h"
#include "Components/GJHInventoryComponent.h"
#include "GameplayTag/GJHGameplayTag.h"
#include "Library/GJHAbilitySystemStatics.h"
#include "Subsystem/GJHUISubSystem.h"
#include "UI/Damage/GJHDamageTextWidget.h"

AGJHPlayerController::AGJHPlayerController()
{
	InventoryComponent = CreateDefaultSubobject<UGJHInventoryComponent>(TEXT("InventoryComponent"));
}

void AGJHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitHUDWidget();
}

void AGJHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(InputMappingContext, 0);

	UGJHInputComponent* GJHInputComponent = CastChecked<UGJHInputComponent>(InputComponent);
	if (IsValid(GJHInputComponent) == false)
		return;

	GJHInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased, &ThisClass::AbilityInputHeld);
	GJHInputComponent->BindAction(InputConfig->MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	GJHInputComponent->BindAction(InputConfig->LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	GJHInputComponent->BindAction(InputConfig->SkillTreeInputAction, ETriggerEvent::Triggered, this, &ThisClass::Input_SkillTree);
	GJHInputComponent->BindAction(InputConfig->InventoryInputAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Inventory);
}

void AGJHPlayerController::InitHUDWidget() const
{
	UGJHUISubSystem* UISubSystem = UGJHUISubSystem::Get(this);
	if (IsValid(UISubSystem) == false)
		return;

	UISubSystem->OpenUI(FGJHGameplayTag::UI_Type_HUD());
}

void AGJHPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (ShouldShowMouseCursor())
		return;
	
	UGJHAbilitySystemComponent* AbilitySystemComponent = UGJHAbilitySystemStatics::GetAbilitySystemComponent(this);
	if (IsValid(AbilitySystemComponent) == false)
		return;

	AbilitySystemComponent->AbilityInputPressed(InputTag);
}

void AGJHPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	UGJHAbilitySystemComponent* AbilitySystemComponent = UGJHAbilitySystemStatics::GetAbilitySystemComponent(this);
	if (IsValid(AbilitySystemComponent) == false)
		return;

	AbilitySystemComponent->AbilityInputReleased(InputTag);
}

void AGJHPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	if (ShouldShowMouseCursor())
		return;
	
	UGJHAbilitySystemComponent* AbilitySystemComponent = UGJHAbilitySystemStatics::GetAbilitySystemComponent(this);
	if (IsValid(AbilitySystemComponent) == false)
		return;

	AbilitySystemComponent->AbilityInputHeld(InputTag);
}

UAbilitySystemComponent* AGJHPlayerController::GetAbilitySystemComponent() const
{
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;
	
	const AGJHPlayerState* GJHPlayerState = GetPlayerState<AGJHPlayerState>();
	if (IsValid(GJHPlayerState))
		AbilitySystemComponent = GJHPlayerState->GetAbilitySystemComponent();
	
	return AbilitySystemComponent;
}

void AGJHPlayerController::Input_Move(const FInputActionValue& Value)
{
	APawn* OwningPawn = GetPawn();
	if (IsValid(OwningPawn) == false)
		return;
	
	FVector2D MoveInput = Value.Get<FVector2D>();
	MoveInput.Normalize();

	const FVector Forward = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X).GetSafeNormal();
	const FVector Right = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y).GetSafeNormal();

	const FVector MoveDirection = Forward * MoveInput.X + Right * MoveInput.Y;
	
	OwningPawn->AddMovementInput(MoveDirection);
}

void AGJHPlayerController::Input_Look(const FInputActionValue& Value)
{
	if (ShouldShowMouseCursor())
		return;
	
	const FVector2D LookInput = Value.Get<FVector2D>();
		
	AddPitchInput(-LookInput.Y);
	AddYawInput(LookInput.X);
}

void AGJHPlayerController::Input_SkillTree(const FInputActionValue& Value)
{
	UGJHUISubSystem* UISubSystem = UGJHUISubSystem::Get(this);
	if (IsValid(UISubSystem))
		UISubSystem->ToggleUI(FGJHGameplayTag::UI_Type_SkillTree());
}

void AGJHPlayerController::Input_Inventory(const FInputActionValue& Value)
{
	UGJHUISubSystem* UISubSystem = UGJHUISubSystem::Get(this);
	if (IsValid(UISubSystem))
		UISubSystem->ToggleUI(FGJHGameplayTag::UI_Type_Inventory());

	InventoryComponent->AddItem(1, 3);
}

void AGJHPlayerController::CreateDamageText(AActor* InTargetActor, float InDamage) const
{	
	if (IsValid(DamageTextComponentClass) == false || IsValid(InTargetActor) == false)
		return;

	UGJHDamageTextComponent* DamageTextComponent = NewObject<UGJHDamageTextComponent>(InTargetActor, DamageTextComponentClass);
	DamageTextComponent->RegisterComponent();
	DamageTextComponent->AttachToComponent(InTargetActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	DamageTextComponent->SetDamage(InDamage);
}
