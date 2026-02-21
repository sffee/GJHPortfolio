#pragma once

#include "GameplayTagContainer.h"
#include "IPropertyTypeCustomization.h"

struct FGJHTableMemo;
class SSearchableComboBox;

class FGJHTableKeyCustomization : public IPropertyTypeCustomization
{
private:
	const FProperty* Property = nullptr;
	TSharedPtr<IPropertyHandle> PropertyHandle;
	TSharedPtr<IPropertyHandle> KeyHandle;
	
	TSharedPtr<SSearchableComboBox> ComboBox;
	TArray<TSharedPtr<FString>> ComboOptions;
	TSharedPtr<STextBlock> ComboBoxText;
	
	TSharedPtr<SEditableTextBox> MemoText;
	
public:	
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FGJHTableKeyCustomization);
	}
	
public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	
private:
	void SetupComboBoxOptions();
	FGameplayTag GetDataTableTag();
	void GetTableMemos(TSet<FGJHTableMemo>& OutTableMemos);
	
	void UpdateKeyText();
	
private:
	TSharedRef<SWidget> OnGenerateComboBox(TSharedPtr<FString> InItem) const;
	void OnSelectionChanged(TSharedPtr<FString> Selection, ESelectInfo::Type SelectInfo);
};
