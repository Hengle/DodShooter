#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PhysicalMaterialWithTags.generated.h"

UCLASS()
class DOD_API UPhysicalMaterialWithTags : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=PhysicalProperties)
	FGameplayTagContainer Tags;
};
