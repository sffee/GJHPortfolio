#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHDamageTextWidget.generated.h"

class UTextBlock;

UCLASS()
class GJHPORTFOLIO_API UGJHDamageTextWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Damage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Animation_DamageText;

public:
	void SetDamage(float InDamage, const FGameplayTag& InStatusTag);
};
