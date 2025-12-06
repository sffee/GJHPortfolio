#include "GJHSkillNodeWidget.h"

#include "GJHSkillSlotWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"

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

void UGJHSkillNodeWidget::SetSkillIndex(int32 InSkillIndex)
{
	SkillIndex = InSkillIndex;
	SkillSlotWidget->SetSlotSkillIndex(SkillIndex);
}

void UGJHSkillNodeWidget::OnClickButton_Add()
{
}

void UGJHSkillNodeWidget::OnClickButton_Sub()
{
}
