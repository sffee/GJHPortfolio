#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GJHUserWidgetBase.generated.h"

class AGJHPlayerState;
class UGJHAbilitySystemComponent;

UCLASS(Abstract, Blueprintable)
class GJHPORTFOLIO_API UGJHUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UGJHAbilitySystemComponent* GetAbilitySystemComponent() const;
	AGJHPlayerState* GetPlayerState() const;
};
