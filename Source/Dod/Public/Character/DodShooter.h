#pragma once

#include "CoreMinimal.h"
#include "DodOperator.h"
#include "DodShooter.generated.h"

class UDodEquipmentManagerComponent;
class UDodInventoryItemDefinition;
class UDodOperatorComponent;

UCLASS()
class DOD_API ADodShooter : public ADodOperator
{
	GENERATED_BODY()

public:
	ADodShooter(const FObjectInitializer& ObjectInitializer);

	void ChangeToFirstPerson();
	void ChangeToThirdPerson();

protected:
	virtual void BeginPlay() override;

	//~Begin APawn interface
	virtual void PossessedBy(AController* NewController) override;
	//~End of APawn interface

	// 添加库存中的每一个物品，然后激活第一个
	void AddInitialInventory();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Character")
	TObjectPtr<USkeletalMeshComponent> ArmMesh;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TArray<TSoftClassPtr<UDodInventoryItemDefinition>> InitialInventoryItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TObjectPtr<UDodEquipmentManagerComponent> EquipmentManager;
};
