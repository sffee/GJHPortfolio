#include "GJHUISubSystem.h"

#include "GJHSettings/GJHUIDeveloperSettings.h"
#include "Library/GJHUIStatics.h"
#include "UI/GJHUserWidgetBase.h"

UGJHUISubSystem* UGJHUISubSystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UGJHUISubSystem>(World->GetGameInstance());
	}

	return nullptr;
}

void UGJHUISubSystem::OpenUI(const FGameplayTag& InUITag)
{
	FGJHUIStateInfo& UIStateInfo = UIStateMap.FindOrAdd(InUITag);
	if (UIStateInfo.bOpen)
		return;

	FGJHUISetting UISetting = UGJHUIStatics::GetUIStateSetting(InUITag);
	if (UISetting.WidgetClass == nullptr)
		return;

	APlayerController* PlayerController = GetPlayerController();
	if (IsValid(PlayerController) == false)
		return;

	UIStateInfo.Widget = CreateWidget(PlayerController, UISetting.WidgetClass.LoadSynchronous());
	UIStateInfo.Widget->AddToViewport();

	UIStateInfo.bOpen = true;

	if (UISetting.bShowMouseCursor)
	{
		++ShowUICursorCount;
		UpdateMouseCursorState();
	}
}

void UGJHUISubSystem::CloseUI(const FGameplayTag& InUITag)
{
	FGJHUIStateInfo& UIStateInfo = UIStateMap.FindOrAdd(InUITag);
	if (UIStateInfo.bOpen == false || IsValid(UIStateInfo.Widget) == false)
		return;


	UIStateInfo.Widget->RemoveFromParent();
	UIStateInfo.bOpen = false;

	FGJHUISetting UISetting = UGJHUIStatics::GetUIStateSetting(InUITag);
	if (UISetting.bShowMouseCursor)
	{
		--ShowUICursorCount;
		UpdateMouseCursorState();
	}
}

void UGJHUISubSystem::ToggleUI(const FGameplayTag& InUITag)
{
	FGJHUIStateInfo& UIStateInfo = UIStateMap.FindOrAdd(InUITag);
	if (UIStateInfo.bOpen)
		CloseUI(InUITag);
	else
		OpenUI(InUITag);
}

void UGJHUISubSystem::UpdateMouseCursorState()
{
	APlayerController* PlayerController = GetPlayerController();
	if (IsValid(PlayerController) == false)
		return;

	if (0 < ShowUICursorCount)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		FInputModeGameOnly InputMode;
		
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
	}
}

APlayerController* UGJHUISubSystem::GetPlayerController() const
{
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = IsValid(GameInstance) ? GameInstance->GetWorld() : nullptr;

	return IsValid(World) ? World->GetFirstPlayerController() : nullptr;
}
