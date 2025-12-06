#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHSkillSlotWidget.generated.h"

class UGJHDraggedSkillSlotWidget;
class UImage;
class USizeBox;

DECLARE_MULTICAST_DELEGATE_OneParam(FGJHOnClickedSkillSlot, class UGJHSkillSlotWidget* SkillSlotWidget);

UCLASS()
class GJHPORTFOLIO_API UGJHSkillSlotWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SkillIcon;

	UPROPERTY(EditAnywhere, Category = "GJH")
	TSubclassOf<UGJHDraggedSkillSlotWidget> DraggedSkillSlotWidgetClass;

private:
	int32 SlotSkillIndex = -1;
	bool bMouseButtonDown = false;

private:
	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FSlateColor DefaultColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f));

	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FSlateColor HoveredColor = FSlateColor(FLinearColor::White);

	UPROPERTY(EditAnywhere, Category = "GJH|Color")
	FSlateColor ClickedColor = FSlateColor(FLinearColor::Gray);

	FSlateColor StateColor = FSlateColor(FLinearColor::White);

public:
	FGJHOnClickedSkillSlot OnClickedSkillSlot;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public:
	void SetSlotSkillIndex(int32 InSkillIndex);
	void SetSlotSkillIcon(int32 InSkillIndex);

	void ClearSlot();
	
private:
	void OnClickedImage_SkillSlot();
	
private:
	FSlateColor GetSkillIconColor(const FSlateColor& InColor) const;
};
