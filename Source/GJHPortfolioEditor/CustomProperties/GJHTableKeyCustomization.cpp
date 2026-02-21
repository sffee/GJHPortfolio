#include "GJHTableKeyCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "SSearchableComboBox.h"
#include "Data/DataTable/GJHDataTableTypes.h"
#include "Subsystem/GJHDataSubSystem.h"
#include "Widgets/Input/SSearchBox.h"

void FGJHTableKeyCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	PropertyHandle = InPropertyHandle;
	Property = PropertyHandle->GetProperty();
	KeyHandle = PropertyHandle->GetChildHandle(TEXT("Key"));
	
	SetupComboBoxOptions();
	
	HeaderRow.NameContent()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.FillWidth(1)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush( TEXT("PropertyWindow.NoOverlayColor")))
			.ForegroundColor_Lambda([this]{ return FLinearColor::Gray; })
			[
				PropertyHandle->CreatePropertyNameWidget()
			]
		]
	]
	.ValueContent()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SAssignNew(ComboBox, SSearchableComboBox)
			.OptionsSource(&ComboOptions)
			.OnSelectionChanged(this, &FGJHTableKeyCustomization::OnSelectionChanged)
			.OnGenerateWidget(this, &FGJHTableKeyCustomization::OnGenerateComboBox)
			.ForegroundColor(FSlateColor::UseForeground())
			.HasDownArrow(true)
			.Content()
			[
				SAssignNew(ComboBoxText, STextBlock)
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5.f, 0.f, 0.f, 0.f)
		[
			SAssignNew(MemoText, SEditableTextBox)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.IsReadOnly(true)
		]
	];
	
	UpdateKeyText();
}

void FGJHTableKeyCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

void FGJHTableKeyCustomization::SetupComboBoxOptions()
{
	ComboOptions.Empty();
	
	TSet<FGJHTableMemo> TableMemos;
	GetTableMemos(TableMemos);
	
	for (const FGJHTableMemo& TableMemo : TableMemos)
	{
		const FString Memo = FString::Printf(TEXT("%d    %s"), TableMemo.Index, *TableMemo.Memo);
		ComboOptions.Add(MakeShareable(new FString(Memo)));
	}
}

FGameplayTag FGJHTableKeyCustomization::GetDataTableTag()
{
	static const FName TableTag_MetaData = FName(TEXT("GJHTableTag"));
	const FString& TableTag = PropertyHandle->GetMetaData(TableTag_MetaData);
	
	return TableTag.IsEmpty() ? FGameplayTag::EmptyTag : FGameplayTag::RequestGameplayTag(FName(TableTag), false);
}

void FGJHTableKeyCustomization::GetTableMemos(TSet<FGJHTableMemo>& OutTableMemos)
{
	OutTableMemos.Empty();
	
	const FGameplayTag TableTag = GetDataTableTag();
	if (TableTag.IsValid() == false)
		return;
	
	TSet<FGJHTableMemo> TableMemos;
	UGJHDataSubSystem::GetTableMemo(TableTag, OutTableMemos);
}

void FGJHTableKeyCustomization::UpdateKeyText()
{
	int32 Index;
	KeyHandle->GetValue(Index);
	
	TSet<FGJHTableMemo> TableMemos;
	GetTableMemos(TableMemos);
	
	if (FGJHTableMemo* Memo = TableMemos.Find(Index))
	{
		ComboBoxText->SetText(FText::AsNumber(Index));
		MemoText->SetText(FText::FromString(Memo->Memo));
	}
}

TSharedRef<SWidget> FGJHTableKeyCustomization::OnGenerateComboBox(TSharedPtr<FString> InItem) const
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem.Get()))
		.Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGJHTableKeyCustomization::OnSelectionChanged(TSharedPtr<FString> Selection, ESelectInfo::Type SelectInfo)
{
	if (Selection.IsValid())
	{
		TArray<FString> V;
		Selection->ParseIntoArray(V, TEXT(" "), true);
		
		int32 Index;
		LexFromString(Index, V[0]);
		KeyHandle->SetValue(Index);
		
		UpdateKeyText();
	}
}
