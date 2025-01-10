#pragma once

#include "CoreMinimal.h"
#include "DodInventoryItemDefinition.h"
#include "GameplayTagContainer.h"
#include "IIF_SetStats.generated.h"

UCLASS()
class DOD_API UIIF_SetStats : public UDodInventoryItemFragment
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UDodInventoryItemInstance* Instance) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;
};
