#pragma once

#include "CoreMinimal.h"
#include "UI/GJHUserWidgetBase.h"
#include "GJHDraggedSkillSlotWidget.generated.h"

class UImage;
class USizeBox;

UCLASS()
class GJHPORTFOLIO_API UGJHDraggedSkillSlotWidget : public UGJHUserWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SkillIcon;

	int32 SlotSkillIndex = -1;

public:
	virtual void NativeOnInitialized() override;

public:
	void SetSize(float InWidth, float InHeight);
	void SetSkillIcon(UTexture2D* InTexture);
	void SetSlotSkillIndex(int32 InSkillIndex);

public:
	FORCEINLINE int32 GetSlotSkillIndex() const { return SlotSkillIndex; }
};
