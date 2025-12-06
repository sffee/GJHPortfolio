#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GJHUISubSystem.generated.h"

USTRUCT()
struct FGJHUIStateInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UUserWidget> Widget;
	
	bool bOpen = false;
};

UCLASS()
class GJHPORTFOLIO_API UGJHUISubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	TMap<FGameplayTag, FGJHUIStateInfo> UIStateMap;

	int32 ShowUICursorCount = 0;
	
public:
	static UGJHUISubSystem* Get(const UObject* WorldContextObject);

public:
	void OpenUI(const FGameplayTag& InUITag);
	void CloseUI(const FGameplayTag& InUITag);
	void ToggleUI(const FGameplayTag& InUITag);

private:
	void UpdateMouseCursorState();

private:
	APlayerController* GetPlayerController() const;
};
