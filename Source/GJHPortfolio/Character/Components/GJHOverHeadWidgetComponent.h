#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GJHOverHeadWidgetComponent.generated.h"

class AGJHCharacterBase;

UCLASS()
class GJHPORTFOLIO_API UGJHOverHeadWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<AGJHCharacterBase> OwningCharacter;
	
public:
	UGJHOverHeadWidgetComponent();

protected:
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void BeginPlay() override;

public:
	bool IsOwnerCharacterDeath() const;
};
