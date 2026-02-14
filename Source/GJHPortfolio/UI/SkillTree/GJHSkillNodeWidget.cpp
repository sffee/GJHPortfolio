#include "GJHSkillNodeWidget.h"

#include "GJHSkillSlotWidget.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "Character/Player/GJHPlayerState.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Data/DataTable/GJHDataTableTypes.h"
#include "Library/GJHDataStatics.h"
#include "Library/GJHGameplayStatics.h"

void UGJHSkillNodeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Button_Add))
		Button_Add->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickButton_Add);

	if (IsValid(Button_Sub))
		Button_Sub->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickButton_Sub);

	if (IsValid(SkillSlotWidget))
	{
		SkillSlotWidget->OnClickedSkillSlot.AddWeakLambda(this, [this](UGJHSkillSlotWidget* SkillSlotWidget)
		{
			OnClickedSkillNode.Broadcast(this);
		});
	}

	if (AGJHPlayerState* PlayerState = GetPlayerState())
		PlayerState->OnChangeSkillPoint.AddUObject(this, &ThisClass::OnChangePoint);
}

void UGJHSkillNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateAddSubButton();
}

void UGJHSkillNodeWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (AGJHPlayerState* PlayerState = GetPlayerState())
		PlayerState->OnChangeSkillPoint.RemoveAll(this);
}

void UGJHSkillNodeWidget::SelectSlot()
{
	if (IsValid(Border_Select))
		Border_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGJHSkillNodeWidget::DeselectSlot()
{
	if (IsValid(Border_Select))
		Border_Select->SetVisibility(ESlateVisibility::Hidden);
}

void UGJHSkillNodeWidget::UpdateAddSubButton()
{
	UGJHAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	FGJHSkillTableInfo SkillTableInfo = UGJHDataStatics::GetSkillInfo(GetOwningPlayer(), SkillIndex);
	if (SkillTableInfo.IsValid() == false || IsValid(ASC) == false)
		return;

	const int32 AbilityLevel = ASC->GetAbilityLevelBySkillIndex(SkillIndex);
	const int32 MinLevel = SkillTableInfo.MinLevel;
	const int32 MaxLevel = SkillTableInfo.MaxLevel;

	const int32 SkillPoint = UGJHGameplayStatics::GetSkillPoint(GetOwningPlayer());
	if (0 < SkillPoint)
	{
		Button_Add->SetIsEnabled(AbilityLevel < MaxLevel);
		Button_Sub->SetIsEnabled(MinLevel < AbilityLevel);
	}
	else
	{
		Button_Add->SetIsEnabled(false);
		Button_Sub->SetIsEnabled(MinLevel < AbilityLevel);
	}
}

void UGJHSkillNodeWidget::SetSkillIndex(int32 InSkillIndex)
{
	SkillIndex = InSkillIndex;
	SkillSlotWidget->SetSlotSkillIndex(SkillIndex);
}

void UGJHSkillNodeWidget::OnClickButton_Add()
{
	UGJHAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC) == false)
		return;

	ASC->AddSkillLevel(SkillIndex, -1);
}

void UGJHSkillNodeWidget::OnClickButton_Sub()
{
	UGJHAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC) == false)
		return;

	ASC->AddSkillLevel(SkillIndex, 1);
}

void UGJHSkillNodeWidget::OnChangePoint(int32 NewPoint)
{
	UpdateAddSubButton();
}
