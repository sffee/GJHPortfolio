#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Tickable.h"
#include "GJHUICosmeticSubsystem.generated.h"

class UGJHOverHeadWidgetComponent;
class APlayerController;

UCLASS()
class GJHPORTFOLIO_API UGJHUICosmeticSubsystem : public ULocalPlayerSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSet<TObjectPtr<UGJHOverHeadWidgetComponent>> OverHeadWidgetComponentSet;

private:
	mutable TWeakObjectPtr<APlayerController> PlayerController; 

public:
	static UGJHUICosmeticSubsystem* Get(const UObject* WorldContextObject);

public:
	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UGJHUICosmeticSubsystem, STATGROUP_Tickables); }
	virtual bool IsTickableWhenPaused() const override { return true; }

public:
	void RegisterOverHeadWidgetComponent(UGJHOverHeadWidgetComponent* InOverHeadWidgetComponent);
	void UnregisterOverHeadWidgetComponent(UGJHOverHeadWidgetComponent* InOverHeadWidgetComponent);

private:
	void UpdateOverHeadWidgetComponent();
};
