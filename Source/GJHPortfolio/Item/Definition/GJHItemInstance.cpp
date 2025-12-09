#include "GJHItemInstance.h"

bool UGJHItemInstance::CanAddToStack(const int32 InAddStack) const
{
	if (ItemDefinition == nullptr)
		return false;

	const int32 MaxStack = ItemDefinition->GetDefaultObject<UGJHItemDefinition>()->GetMaxStack();
	if (MaxStack < Stack + InAddStack)
		return false;

	return true;
}

void UGJHItemInstance::AddToStack(const int32 InAddStack)
{
	if (ItemDefinition == nullptr)
		return;

	const int32 MaxStack = ItemDefinition->GetDefaultObject<UGJHItemDefinition>()->GetMaxStack();
	Stack = FMath::Clamp(Stack + InAddStack, 0, MaxStack);
}
