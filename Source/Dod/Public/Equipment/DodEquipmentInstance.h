#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DodEquipmentInstance.generated.h"

struct FDodEquipmentActorToSpawn;

UCLASS(BlueprintType, Blueprintable)
class DOD_API UDodEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;

	virtual void OnEquipped();
	virtual void OnUnequipped();

	virtual void SpawnEquipmentActors(const TArray<FDodEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnUnequipped"))
	void K2_OnUnequipped();

private:
	UFUNCTION()
	void OnRep_Instigator();
	UPROPERTY(ReplicatedUsing=OnRep_Instigator)
	TObjectPtr<UObject> Instigator;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
