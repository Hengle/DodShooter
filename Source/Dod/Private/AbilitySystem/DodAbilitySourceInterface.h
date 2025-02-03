#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DodAbilitySourceInterface.generated.h"

struct FGameplayTagContainer;

UINTERFACE()
class UDodAbilitySourceInterface : public UInterface
{
	GENERATED_BODY()
};

class IDodAbilitySourceInterface
{
	GENERATED_BODY()

public:
	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial,
	                                             const FGameplayTagContainer* SourceTags = nullptr,
	                                             const FGameplayTagContainer* TargetTags = nullptr) const = 0;
};
