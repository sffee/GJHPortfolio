#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHSkillNodeWidget.generated.h"

class UGJHSkillSlotWidget;
class UBorder;
class UButton;
class USizeBox;

DECLARE_MULTICAST_DELEGATE_OneParam(FGJHOnClickedSkillNode, class UGJHSkillNodeWidget* SkillNodeWidget);

UCLASS()
class GJHPORTFOLIO_API UGJHSkillNodeWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_SkillIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Add;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Sub;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Select;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGJHSkillSlotWidget> SkillSlotWidget;

private:
	UPROPERTY(EditAnywhere, Category = "GJH")
	int32 SkillIndex = -1;

private:
	bool bMouseButtonDown;

public:
	FGJHOnClickedSkillNode OnClickedSkillNode;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SelectSlot();
	void DeselectSlot();

private:
	void UpdateAddSubButton();

public:
	void SetSkillIndex(int32 InSkillIndex);
	FORCEINLINE int32 GetSkillIndex() const { return SkillIndex; }
	
private:
	UFUNCTION()
	void OnClickButton_Add();

	UFUNCTION()
	void OnClickButton_Sub();

	void OnChangePoint(int32 NewPoint);
};
