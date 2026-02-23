#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHActorStatusBubbleWidget.generated.h"

class UGJHActorStatusBubbleIconWidget;
class UWrapBox;

UCLASS()
class GJHPORTFOLIO_API UGJHActorStatusBubbleWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> WrapBox_Bubble;
	
	UPROPERTY(EditAnywhere, Category = "Status")
	TObjectPtr<UMaterialInterface> BubbleMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Status")
	TSubclassOf<UGJHActorStatusBubbleIconWidget> BubbleIconWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MID;
	
	FGameplayTag StatusTag;
	FGameplayTag LastStatusTag;
	
public:
	void InitStatus(const FGameplayTag& InStatusTag, int32 InCount);
	void RefreshStatus(const FGameplayTag& InStatusTag, int32 InCount);
	
public:
	FGameplayTag GetStatusTag() const { return StatusTag; }
};
