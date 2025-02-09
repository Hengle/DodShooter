#pragma once

#include "CoreMinimal.h"
#include "DodInputConfig.h"
#include "EnhancedInputComponent.h"
#include "DodInputComponent.generated.h"

UCLASS(Config = Input)
class DOD_API UDodInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename FuncType>
	void BindNativeAction(const UDodInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent,
	                      UserClass* Object, FuncType Func);

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UDodInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                        ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template <class UserClass, typename FuncType>
void UDodInputComponent::BindNativeAction(const UDodInputConfig* InputConfig, const FGameplayTag& InputTag,
                                          ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UDodInputComponent::BindAbilityActions(const UDodInputConfig* InputConfig, UserClass* Object,
                                            PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                            TArray<uint32>& BindHandles)
{
	check(InputConfig);
	
	for (const FDodInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc,
				                           Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc,
				                           Action.InputTag).GetHandle());
			}
		}
	}
}
