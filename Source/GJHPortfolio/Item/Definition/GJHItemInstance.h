#pragma once

#include "CoreMinimal.h"
#include "GJHItemDefinition.h"
#include "Data/DataTable/GJHDataTableTypes.h"
#include "UObject/Object.h"
#include "GJHItemInstance.generated.h"

UCLASS()
class GJHPORTFOLIO_API UGJHItemInstance : public UObject
{
	GENERATED_BODY()

private:
	int32 ItemIndex = INVALID_ITEM_INDEX;
	TSubclassOf<UGJHItemDefinition> ItemDefinition;

	int32 Stack = 0;

private:
	int32 InventorySlotIndex = INVALID_ITEM_INDEX;

public:
	FORCEINLINE void SetItemDefinition(TSubclassOf<UGJHItemDefinition> InItemDefinition) { ItemDefinition = InItemDefinition; }
	
	bool CanAddToStack(const int32 InAddStack) const;
	void AddToStack(const int32 InAddStack);

public:
	FORCEINLINE void SetInventorySlotIndex(const int32 InSlotIndex) { InventorySlotIndex = InSlotIndex; }
	
public:
	FORCEINLINE int32 GetItemIndex() const { return ItemIndex; }
	FORCEINLINE int32 GetStack() const { return Stack; }
	FORCEINLINE int32 GetInventorySlotIndex() const { return InventorySlotIndex; }
	FORCEINLINE TSubclassOf<UGJHItemDefinition> GetItemDefinitionClass() const { return ItemDefinition; }
	FORCEINLINE UGJHItemDefinition* GetItemDefinition() const { return ItemDefinition->GetDefaultObject<UGJHItemDefinition>(); }
};
