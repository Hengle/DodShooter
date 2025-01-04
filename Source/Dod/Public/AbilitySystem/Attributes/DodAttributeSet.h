﻿#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "DodAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FGameplayEffectSpec;
DECLARE_MULTICAST_DELEGATE_SixParams(FDodAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/,
                                     const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/,
                                     float /*OldValue*/, float /*NewValue*/);

UCLASS()
class DOD_API UDodAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
};
