#include "GJHOverlaySkillSlotWidget.h"

#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "AbilitySystem/GJHGameplayAbilityBase.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Library/GJHAbilitySystemStatics.h"
#include "Library/GJHDataStatics.h"
#include "Library/GJHGameplayStatics.h"
#include "UI/SkillTree/GJHDraggedSkillSlotWidget.h"
#include "UI/SkillTree/GJHSkillSlotWidget.h"

UGJHOverlaySkillSlotWidget::UGJHOverlaySkillSlotWidget()
{
}

void UGJHOverlaySkillSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (UGJHAbilitySystemComponent* ASC = UGJHAbilitySystemStatics::GetAbilitySystemComponent(GetOwningPlayer()))
	{
		ASC->OnAbilityGiven.AddUObject(this, &ThisClass::OnAbilityGiven);
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnActiveEffectApplied);
		ASC->OnAbilityEquipped.AddUObject(this, &ThisClass::OnAbilityEquipped);
	}
}

void UGJHOverlaySkillSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (QuickSlotInputTag.IsValid())
	{
		if (UGJHAbilitySystemComponent* ASC = UGJHAbilitySystemStatics::GetAbilitySystemComponent(GetOwningPlayer()))
		{
			FGameplayAbilitySpec* AbilitySpec = ASC->GetSpecByQuickSlotInputTag(QuickSlotInputTag);
			if (AbilitySpec)
				UpdateSlot(AbilitySpec);
		}
	}
}

void UGJHOverlaySkillSlotWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (UGJHAbilitySystemComponent* ASC = UGJHAbilitySystemStatics::GetAbilitySystemComponent(GetOwningPlayer()))
	{
		ASC->OnAbilityGiven.RemoveAll(this);
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		ASC->OnAbilityEquipped.RemoveAll(this);
	}
}

void UGJHOverlaySkillSlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UGJHDraggedSkillSlotWidget* DraggedSlotWidget = Cast<UGJHDraggedSkillSlotWidget>(InOperation->DefaultDragVisual);
	if (IsValid(DraggedSlotWidget))
		Border_DragOver->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGJHOverlaySkillSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Border_DragOver->SetVisibility(ESlateVisibility::Hidden);
}

bool UGJHOverlaySkillSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Border_DragOver->SetVisibility(ESlateVisibility::Hidden);

	const UGJHDraggedSkillSlotWidget* DraggedSlotWidget = Cast<UGJHDraggedSkillSlotWidget>(InOperation->DefaultDragVisual);
	if (IsValid(DraggedSlotWidget) == false)
		return false;

	if (UGJHAbilitySystemComponent* ASC = UGJHAbilitySystemStatics::GetAbilitySystemComponent(GetOwningPlayer()))
		ASC->EquipAbility(DraggedSlotWidget->GetSlotSkillIndex(), QuickSlotInputTag);

	return true;
}

void UGJHOverlaySkillSlotWidget::UpdateSlot(const int32 InSkillIndex, const FGameplayTag& InEquipQuickSlotInputTag, const FGameplayTag& PrevQuickSlotInputTag)
{
	if (PrevQuickSlotInputTag.MatchesTagExact(QuickSlotInputTag))
	{
		ClearSlot();
		return;
	}

	if (QuickSlotInputTag.MatchesTagExact(InEquipQuickSlotInputTag))
	{
		SkillSlotWidget->SetSlotSkillIndex(InSkillIndex);
		SkillSlotWidget->SetRenderOpacity(1.f);

		SlotSkillIndex = InSkillIndex;
		CooldownTag = UGJHDataStatics::GetSkillCooldownTag(this, SlotSkillIndex);

		if (UGJHAbilitySystemComponent* ASC = UGJHAbilitySystemStatics::GetAbilitySystemComponent(GetOwningPlayer()))
		{
			FGameplayTagContainer GameplayTagContainer;
			ASC->GetOwnedGameplayTags(GameplayTagContainer);
			
			ProgressCooldown(GameplayTagContainer);
		}
	}
}

void UGJHOverlaySkillSlotWidget::UpdateSlot(const FGameplayAbilitySpec* InAbilitySpec)
{
	if (UGJHAbilitySystemComponent* ASC = UGJHAbilitySystemStatics::GetAbilitySystemComponent(GetOwningPlayer()))
	{
		const int32 SkillIndex = UGJHDataStatics::GetSkillIndex(this, InAbilitySpec->Ability);
		const FGameplayTag InputTag = ASC->GetQuickSlotInputTagFromSpec(InAbilitySpec);
		UpdateSlot(SkillIndex, InputTag, FGameplayTag::EmptyTag);
	}
}

void UGJHOverlaySkillSlotWidget::ClearSlot()
{
	SlotSkillIndex = -1;
	CooldownTag = FGameplayTag::EmptyTag;
	SkillSlotWidget->SetRenderOpacity(0.f);
	SkillSlotWidget->ClearSlot();
	Progress_Cooldown->SetPercent(0.f);
	TextBlock_Cooldown->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
}

void UGJHOverlaySkillSlotWidget::UpdateCooldown()
{
	ReminingCooldown -= 0.1f;
	if (ReminingCooldown <= 0.f)
	{
		Progress_Cooldown->SetPercent(0.f);
		TextBlock_Cooldown->SetVisibility(ESlateVisibility::Hidden);

		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);

		if (IsValid(Animation_EndCooldown))
			PlayAnimation(Animation_EndCooldown);

		return;
	}

	Progress_Cooldown->SetPercent(ReminingCooldown / Cooldown);
	TextBlock_Cooldown->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), ReminingCooldown)));
}

void UGJHOverlaySkillSlotWidget::OnActiveEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	GameplayEffectSpec.GetAllAssetTags(GameplayTagContainer);
	GameplayEffectSpec.GetAllGrantedTags(GameplayTagContainer);

	ProgressCooldown(GameplayTagContainer);
}

void UGJHOverlaySkillSlotWidget::ProgressCooldown(const FGameplayTagContainer& InGameplayTagContainer)
{
	if (InGameplayTagContainer.HasTagExact(CooldownTag))
	{
		if (UGJHAbilitySystemComponent* ASC = UGJHAbilitySystemStatics::GetAbilitySystemComponent(GetOwningPlayer()))
		{
			FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
			TArray<float> TimesDurations = ASC->GetActiveEffectsDuration(GameplayEffectQuery);
			TArray<float> TimesRemainings = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
			if (TimesDurations.IsEmpty() == false && TimesRemainings.IsEmpty() == false)
			{
				Cooldown = TimesDurations[0];
				ReminingCooldown = TimesRemainings[0];
				if (0.f < Cooldown && IsValid(Border_Cooldown) && IsValid(Progress_Cooldown) && IsValid(TextBlock_Cooldown))
				{
					Border_Cooldown->SetRenderOpacity(1.f);
					Border_Cooldown->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));
					Border_Cooldown->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Progress_Cooldown->SetPercent(ReminingCooldown / Cooldown);
					TextBlock_Cooldown->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					TextBlock_Cooldown->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), ReminingCooldown)));

					GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ThisClass::UpdateCooldown, 0.1f, true);
				}
			}
		}
	}
}

void UGJHOverlaySkillSlotWidget::OnAbilityGiven(const FGameplayAbilitySpec& AbilitySpec)
{
	UpdateSlot(&AbilitySpec);
}

void UGJHOverlaySkillSlotWidget::OnAbilityEquipped(int32 SkillIndex, FGameplayTag EquipQuickSlotInputTag, FGameplayTag PrevQuickSlotInputTag)
{
	UpdateSlot(SkillIndex, EquipQuickSlotInputTag, PrevQuickSlotInputTag);
}
