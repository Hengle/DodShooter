#pragma once

#include "CoreMinimal.h"
#include "DodInventoryItemDefinition.h"
#include "IIF_EquippableItem.generated.h"

class UDodEquipmentDefinition;

UCLASS()
class DOD_API UIIF_EquippableItem : public UDodInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Dod)
	TSubclassOf<UDodEquipmentDefinition> EquipmentDefinition;
};
