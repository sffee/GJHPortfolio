#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHSkillTreeWidget.generated.h"

class UWrapBox;
class UGJHSkillSlotWidget;
class URichTextBlock;
class UGJHSkillNodeWidget;
class USizeBox;
class UTextBlock;

UCLASS()
class GJHPORTFOLIO_API UGJHSkillTreeWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SkillPoint;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> WrapBox_SkillNode;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> RichTextBlock_Description;
	
private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	TSubclassOf<UGJHSkillNodeWidget> SkillNodeWidgetClass;

private:
	UPROPERTY()
	TArray<TObjectPtr<UGJHSkillNodeWidget>> SkillNodes;

private:
	UPROPERTY()
	TObjectPtr<UGJHSkillNodeWidget> SelectedNode;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	void InitSkillNode();
	void UpdateDescription() const;

private:
	void OnClickedSkillNode(UGJHSkillNodeWidget* SkillNodeWidget);
	void OnChangeSkillPoint(int32 NewPoint);
};
