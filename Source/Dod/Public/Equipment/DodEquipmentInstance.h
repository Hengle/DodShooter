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
	//~ Begin UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; };
	virtual UWorld* GetWorld() const override;
	//~ End UObject interface

	UFUNCTION(BlueprintPure, Category=Equipment)
	UObject* GetInstigator() { return Instigator; }

	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, Category=Equipment)
	AActor* GetSpawnedActor() const { return SpawnedActor; }

	virtual void OnEquipped();
	virtual void OnUnequipped();

	virtual void SpawnEquipmentActors(const FDodEquipmentActorToSpawn& ActorToSpawn);
	virtual void DestroyEquipmentActors();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

#if UE_WITH_IRIS
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
	                                          UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
#endif
	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnUnequipped"))
	void K2_OnUnequipped();

	UPROPERTY(Replicated)
	TObjectPtr<AActor> SpawnedActor;

private:
	UFUNCTION()
	void OnRep_Instigator();
	UPROPERTY(ReplicatedUsing=OnRep_Instigator)
	TObjectPtr<UObject> Instigator;
};
