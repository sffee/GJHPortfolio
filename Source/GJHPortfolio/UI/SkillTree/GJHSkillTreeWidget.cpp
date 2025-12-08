#include "GJHSkillTreeWidget.h"

#include "GJHSkillNodeWidget.h"
#include "AbilitySystem/GJHAbilitySystemComponent.h"
#include "Character/Player/GJHPlayerState.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Library/GJHDataStatics.h"
#include "Library/GJHGameplayStatics.h"

void UGJHSkillTreeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UGJHSkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitSkillNode();

	if (AGJHPlayerState* PS = UGJHGameplayStatics::GetGJHPlayerState(this))
	{
		TextBlock_SkillPoint->SetText(FText::AsNumber(PS->GetSkillPoint()));
		PS->OnChangeSkillPoint.AddUObject(this, &UGJHSkillTreeWidget::OnChangeSkillPoint);
	}
}

void UGJHSkillTreeWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (AGJHPlayerState* PS = UGJHGameplayStatics::GetGJHPlayerState(this))
	{
		PS->OnChangeSkillPoint.RemoveAll(this);
	}
}

FReply UGJHSkillTreeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UGJHSkillTreeWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UGJHSkillTreeWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

void UGJHSkillTreeWidget::InitSkillNode()
{
	if (SkillNodeWidgetClass == nullptr)
		return;
	
	WrapBox_SkillNode->ClearChildren();

	UGJHDataStatics::ForeachSkillInfo(this, [this](const FGJHSkillTableInfo& InSkillInfo)
	{
		UGJHSkillNodeWidget* SkillNodeWidget = CreateWidget<UGJHSkillNodeWidget>(GetOwningPlayer(), SkillNodeWidgetClass);
		SkillNodeWidget->SetSkillIndex(InSkillInfo.Index);
		SkillNodeWidget->OnClickedSkillNode.AddUObject(this, &ThisClass::OnClickedSkillNode);
		
		WrapBox_SkillNode->AddChildToWrapBox(SkillNodeWidget);
	});
}

void UGJHSkillTreeWidget::UpdateDescription() const
{
	if (GetAbilitySystemComponent() && IsValid(SelectedNode))
	{
		const FString Description = GetAbilitySystemComponent()->GetDescriptionBySkillIndex(SelectedNode->GetSkillIndex());

		if (RichTextBlock_Description)
			RichTextBlock_Description->SetText(FText::FromString(Description));
	}
}

void UGJHSkillTreeWidget::OnClickedSkillNode(UGJHSkillNodeWidget* SkillNodeWidget)
{
	if (SelectedNode == SkillNodeWidget)
		return;

	if (SelectedNode)
		SelectedNode->DeselectSlot();

	SkillNodeWidget->SelectSlot();
	SelectedNode = SkillNodeWidget;

	UpdateDescription();
}

void UGJHSkillTreeWidget::OnChangeSkillPoint(int32 NewPoint)
{
	TextBlock_SkillPoint->SetText(FText::AsNumber(NewPoint));

	UpdateDescription();
}
