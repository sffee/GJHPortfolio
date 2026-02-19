#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"

#include "BakeAttackAnimSequenceWidget.generated.h"

class UGJHBakeAttackAnimSequenceDataAsset;
class UEditorUtilityButton;
class UAnimSequence;

UCLASS()
class GJHPORTFOLIOEDITOR_API UBakeAttackAnimSequenceWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditorUtilityButton> EditorUtilityButton_Bake;

private:
	UPROPERTY(EditAnywhere, Category = "Bake")
	TObjectPtr<UAnimMontage> AnimMontage;
	
	UPROPERTY(EditAnywhere, Category = "Bake")
	float SamplingInterval = 0.05f;
	
	UPROPERTY(EditAnywhere, Category = "Bake")
	FName StartSocketName = FName("Socket_Attack1");
	
	UPROPERTY(EditAnywhere, Category = "Bake")
	FName EndSocketName = FName("Socket_Attack2");
	
private:
	static const FString SaveDirectoryPath;
	
public:
	virtual void NativeConstruct() override;
	
private:
	UGJHBakeAttackAnimSequenceDataAsset* CreateDataAsset(const FString& InAssetName);
	void SaveDataAsset(UDataAsset* InDataAsset);
	
private:
	UFUNCTION()
	void OnClickedButton_Bake();
};
