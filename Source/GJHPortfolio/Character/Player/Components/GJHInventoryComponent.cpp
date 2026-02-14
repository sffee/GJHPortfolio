#include "GJHInventoryComponent.h"

#include "GameplayTag/GJHGameplayTag.h"
#include "Item/Definition/GJHItemDefinition.h"
#include "Item/Definition/GJHItemInstance.h"
#include "Library/GJHDataStatics.h"
#include "Library/GJHUIStatics.h"
#include "UI/Inventory/GJHInventoryGridWidget.h"
#include "UI/Inventory/GJHInventoryWidget.h"

UGJHInventoryComponent::UGJHInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGJHInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGJHInventoryComponent::AddItem(const int32 InItemIndex, const int32 InStack)
{
	TSubclassOf<UGJHItemDefinition> ItemDefinition = UGJHDataStatics::GetItemDefinition(this, InItemIndex);
	if (ItemDefinition == nullptr)
	{
		ensure(false);
		return;
	}

	UGJHInventoryWidget* InventoryWidget = UGJHUISubSystem::Get(this)->GetWidget<UGJHInventoryWidget>(FGJHGameplayTag::UI_Type_Inventory());
	UGJHInventoryGridWidget* GridWidget = IsValid(InventoryWidget) ? InventoryWidget->GetGridWidget() : nullptr;
	if (IsValid(GridWidget) == false)
		return;

	if (ItemDefinition->GetDefaultObject<UGJHItemDefinition>()->IsStackable())
	{
		GridWidget->AddStackableItem(ItemDefinition, InStack, ItemInstances);
	}
	else
	{
		if (UGJHItemInstance* AddedItemInstance = GridWidget->AddEquipmentItem(ItemDefinition); IsValid(AddedItemInstance))
		{
			ItemInstances.Add(AddedItemInstance);
		}
	}
}

void UGJHInventoryComponent::UpdateItemSlotIndex(UGJHItemInstance* InItemInstance, const int32 InNewSlotIndex)
{
	for (UGJHItemInstance* ItemInstance : ItemInstances)
	{
		if (ItemInstance == InItemInstance)
		{
			ItemInstance->SetInventorySlotIndex(InNewSlotIndex);
			return;
		}
	}
}
