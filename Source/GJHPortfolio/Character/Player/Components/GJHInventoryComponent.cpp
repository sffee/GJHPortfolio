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

UGJHItemInstance* UGJHInventoryComponent::AddItem(const int32 InItemIndex)
{
	TSubclassOf<UGJHItemDefinition> ItemDefinition = UGJHDataStatics::GetItemDefinition(this, InItemIndex);
	if (ItemDefinition == nullptr)
	{
		ensure(false);
		return nullptr;
	}

	UGJHInventoryWidget* InventoryWidget = UGJHUISubSystem::Get(this)->GetWidget<UGJHInventoryWidget>(FGJHGameplayTag::UI_Type_Inventory());
	UGJHInventoryGridWidget* GridWidget = IsValid(InventoryWidget) ? InventoryWidget->GetGridWidget() : nullptr;
	if (IsValid(GridWidget) == false)
		return nullptr;

	if (UGJHItemInstance* AddedItemInstance = GridWidget->AddItem(ItemDefinition); IsValid(AddedItemInstance))
	{
		ItemInstances.Add(AddedItemInstance);
		return AddedItemInstance;
	}

	return nullptr;
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
