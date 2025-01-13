#pragma once

#include "CoreMinimal.h"
#include "DodOperator.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "DodShooter.generated.h"

class UDodEquipmentManagerComponent;
class UDodInventoryItemDefinition;
class UDodOperatorComponent;

UCLASS()
class DOD_API ADodShooter : public ADodOperator, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	ADodShooter(const FObjectInitializer& ObjectInitializer);

	void ChangeToFirstPerson();
	void ChangeToThirdPerson();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	                                FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	                                   FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End of IGameFrameworkInitStateInterface interface

	// 添加库存中的每一个物品，然后激活第一个
	void AddInitialInventory();

public:
	static const FName NAME_ActorFeatureName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Character")
	TObjectPtr<USkeletalMeshComponent> ArmMesh;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TArray<TSoftClassPtr<UDodInventoryItemDefinition>> InitialInventoryItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TObjectPtr<UDodEquipmentManagerComponent> EquipmentManager;
};
