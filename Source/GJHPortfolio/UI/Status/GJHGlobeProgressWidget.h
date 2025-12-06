#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHGlobeProgressWidget.generated.h"

class UProgressBar;

UCLASS()
class GJHPORTFOLIO_API UGJHGlobeProgressWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Globe;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_GhostGlobe;

private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TObjectPtr<UMaterialInterface> GlobeMaterial;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TObjectPtr<UMaterialInterface> GhostGlobeMaterial;

private:
	float CurrentGhostGlobePercent;
	float TargetGhostGlobePercent;

public:
	UGJHGlobeProgressWidget();

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetGlobePercent(float InPercent);
};
