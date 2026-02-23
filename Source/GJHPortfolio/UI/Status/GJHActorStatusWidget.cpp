#include "GJHActorStatusWidget.h"

#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "Components/VerticalBox.h"
#include "GameplayTag/GJHGameplayTag.h"
#include "Status/GJHActorStatusBubbleWidget.h"

void UGJHActorStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGJHActorStatusWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (OwningAbilitySystemComponent.IsValid() && OnGameplayEffectAppliedDelegateHandle.IsValid())
	{
		OwningAbilitySystemComponent->OnStatusTagChanged.Remove(OnStatusTagChangedDelegateHandle);
	}
}

void UGJHActorStatusWidget::SetAbilitySystemComponent(UGJHAbilitySystemComponent* InAbilitySystemComponent)
{
	OwningAbilitySystemComponent = InAbilitySystemComponent;
	
	if (OwningAbilitySystemComponent.IsValid())
	{
		OnStatusTagChangedDelegateHandle = OwningAbilitySystemComponent->OnStatusTagChanged.AddUObject(this, &ThisClass::OnStatusTagUpdated);
	}
}

void UGJHActorStatusWidget::OnStatusTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (Tag.MatchesTagExact(FGJHGameplayTag::Status_Type()) || Tag.MatchesTag(FGJHGameplayTag::Status_Type()) == false)
		return;
	
	auto FindBubbleWidget = [this](const FGameplayTag& InStatusTag) -> UGJHActorStatusBubbleWidget*
	{
		for (UGJHActorStatusBubbleWidget* BubbleWidget : BubbleWidgets)
		{
			const FGameplayTag StatusTag = BubbleWidget->GetStatusTag();
			if (InStatusTag.MatchesTag(StatusTag))
				return BubbleWidget;
		}
		
		return nullptr;
	};
	
	if (0 < NewCount)
	{
		if (UGJHActorStatusBubbleWidget* FoundBubbleWidget = FindBubbleWidget(Tag))
		{
			FoundBubbleWidget->RefreshStatus(Tag, NewCount);
			return;
		}
		
		UGJHActorStatusBubbleWidget* BubbleWidget = CreateWidget<UGJHActorStatusBubbleWidget>(GetWorld(), StatusBubbleWidgetClass);
		BubbleWidget->InitStatus(Tag, NewCount);
		
		VerticalBox_Status->AddChildToVerticalBox(BubbleWidget);
		BubbleWidgets.Add(BubbleWidget);
	}
	else
	{
		if (UGJHActorStatusBubbleWidget* FoundBubbleWidget = FindBubbleWidget(Tag))
		{
			VerticalBox_Status->RemoveChild(FoundBubbleWidget);
			BubbleWidgets.Remove(FoundBubbleWidget);
		}
	}
}
