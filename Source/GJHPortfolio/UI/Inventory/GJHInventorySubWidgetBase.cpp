#include "GJHInventorySubWidgetBase.h"

#include "Data/DataTable/GJHDataTableTypes.h"
#include "Item/Definition/GJHItemInstance.h"

void UGJHInventorySubWidgetBase::AddStack(const int32 InAddStack)
{
	ItemInstance->AddStack(InAddStack);
}

void UGJHInventorySubWidgetBase::SetItemInstance(UGJHItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
}

UGJHItemDefinition* UGJHInventorySubWidgetBase::GetItemDefinition() const
{
	return ItemInstance->GetItemDefinition();
}

TSubclassOf<UGJHItemDefinition> UGJHInventorySubWidgetBase::GetItemDefinitionClass() const
{
	return ItemInstance->GetItemDefinitionClass();
}

int32 UGJHInventorySubWidgetBase::GetStack() const
{
	return ItemInstance->GetStack();
}

FIntPoint UGJHInventorySubWidgetBase::GetGridSize() const
{
	return ItemInstance.IsValid() ? ItemInstance->GetItemDefinition()->GetGridSize() : FIntPoint::ZeroValue;
}

UTexture2D* UGJHInventorySubWidgetBase::GetItemIcon() const
{
	return GetItemDefinition()->GetIcon();
}

int32 UGJHInventorySubWidgetBase::GetStackRemainder() const
{
	return GetItemDefinition()->GetMaxStack() - GetItemInstance()->GetStack();
}

bool UGJHInventorySubWidgetBase::IsValidItem() const
{
	return ItemInstance.IsValid() && SlotIndex != INVALID_ITEM_INDEX && LeftTopIndex != INVALID_ITEM_INDEX;
}

bool UGJHInventorySubWidgetBase::IsStackable() const
{
	return ItemInstance.IsValid() ? ItemInstance->IsStackable() : false;
}
