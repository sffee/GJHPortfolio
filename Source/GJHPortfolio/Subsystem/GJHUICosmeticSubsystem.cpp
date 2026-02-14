#include "GJHUICosmeticSubsystem.h"

#include "Character/Components/GJHOverHeadWidgetComponent.h"
#include "GameFramework/Character.h"
#include "GJHSettings/GJHUIDeveloperSettings.h"

UGJHUICosmeticSubsystem* UGJHUICosmeticSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		UGameInstance* GameInstance = IsValid(World) ? World->GetGameInstance() : nullptr;
		ULocalPlayer* LocalPlayer = IsValid(GameInstance) ? GameInstance->GetFirstGamePlayer() : nullptr;
		return IsValid(LocalPlayer) ? LocalPlayer->GetSubsystem<UGJHUICosmeticSubsystem>() : nullptr;
	}

	return nullptr;
}

bool UGJHUICosmeticSubsystem::IsTickable() const
{
	if (IsTemplate())
		return false;

	PlayerController = GetLocalPlayer()->GetPlayerController(nullptr);
	return PlayerController.IsValid() && IsValid(PlayerController->GetCharacter());
}

void UGJHUICosmeticSubsystem::Tick(float DeltaTime)
{
	UpdateOverHeadWidgetComponent();
}

void UGJHUICosmeticSubsystem::RegisterOverHeadWidgetComponent(UGJHOverHeadWidgetComponent* InOverHeadWidgetComponent)
{
	OverHeadWidgetComponentSet.Add(InOverHeadWidgetComponent);
}

void UGJHUICosmeticSubsystem::UnregisterOverHeadWidgetComponent(UGJHOverHeadWidgetComponent* InOverHeadWidgetComponent)
{
	OverHeadWidgetComponentSet.Remove(InOverHeadWidgetComponent);
}

void UGJHUICosmeticSubsystem::UpdateOverHeadWidgetComponent()
{
	if (OverHeadWidgetComponentSet.IsEmpty())
		return;
	
	ACharacter* PlayerCharacter = PlayerController->GetCharacter();
	if (IsValid(PlayerCharacter) == false)
		return;

	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const float UpdateThreshold = GetDefault<UGJHUIDeveloperSettings>()->OverHeadWidgetThreshold;

	for (UGJHOverHeadWidgetComponent* OverHeadWidgetComponent : OverHeadWidgetComponentSet)
	{
		if (OverHeadWidgetComponent->IsOwnerCharacterDeath())
		{
			if (OverHeadWidgetComponent->IsVisible())
				OverHeadWidgetComponent->SetVisibility(false);

			return;
		}
		
		const AActor* OwningActor = OverHeadWidgetComponent->GetOwner();
		
		const float Distance = FVector::Distance(PlayerLocation, OwningActor->GetActorLocation());
		const bool bVisible = Distance <= UpdateThreshold;

		if (OverHeadWidgetComponent->IsVisible() != bVisible)
			OverHeadWidgetComponent->SetVisibility(bVisible);
	}
}
