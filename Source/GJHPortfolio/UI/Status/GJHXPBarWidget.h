#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHXPBarWidget.generated.h"

class UProgressBar;

UCLASS()
class GJHPORTFOLIO_API UGJHXPBarWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_XP;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	void OnXPPercentChanged(float NewPercent);
};
