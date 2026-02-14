#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Data/DataAsset/GJHInputConfigDataAsset.h"
#include "GJHInputComponent.generated.h"

class UGJHInputConfigDataAsset;

UCLASS()
class GJHPORTFOLIO_API UGJHInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UGJHInputComponent();

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UGJHInputConfigDataAsset* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UGJHInputComponent::BindAbilityActions(const UGJHInputConfigDataAsset* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const auto& [InputTag, InputAction] : InputConfig->AbilityInputActionMap)
	{
		if (InputTag.IsValid() && IsValid(InputAction))
		{
			if (PressedFunc)
			{
				BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputTag);
			}

			if (HeldFunc)
			{
				BindAction(InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputTag);
			}
		}
	}
}
