#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHActorStatusWidget.generated.h"

class UVerticalBox;
class UGJHAbilitySystemComponent;
class UGJHActorStatusBubbleWidget;

UCLASS()
class GJHPORTFOLIO_API UGJHActorStatusWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Status;
	
private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	TSubclassOf<UGJHActorStatusBubbleWidget> StatusBubbleWidgetClass;
	
	UPROPERTY()
	TWeakObjectPtr<UGJHAbilitySystemComponent> OwningAbilitySystemComponent;
	
	UPROPERTY()
	TArray<UGJHActorStatusBubbleWidget*> BubbleWidgets;
	
	FDelegateHandle OnGameplayEffectAppliedDelegateHandle;
	FDelegateHandle OnStatusTagChangedDelegateHandle;
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
public:
	void SetAbilitySystemComponent(UGJHAbilitySystemComponent* InAbilitySystemComponent);
	
private:
	void OnStatusTagUpdated(const FGameplayTag Tag, int32 NewCount);
};
