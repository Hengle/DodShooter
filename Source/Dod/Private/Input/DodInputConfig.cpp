#include "Input/DodInputConfig.h"

UDodInputConfig::UDodInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UDodInputConfig::FindNativeInputActionForTag(const FGameplayTag& InInputTag) const
{
	for (const auto& [InputAction, InputTag] : NativeInputActions)
	{
		if (InputAction && InputTag == InInputTag)
		{
			return InputAction;
		}
	}

	return nullptr;
}

const UInputAction* UDodInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InInputTag) const
{
	for (const auto& [InputAction, InputTag] : AbilityInputActions)
	{
		if (InputAction && InputTag == InInputTag)
		{
			return InputAction;
		}
	}

	return nullptr;
}
