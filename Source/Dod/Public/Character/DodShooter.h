#pragma once

#include "CoreMinimal.h"
#include "DodOperator.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "DodShooter.generated.h"

class UDodExperienceDefinition;
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

	void ChooseViewPerson();

protected:
	//~ Begin ADodCharacter interface
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void DestroyDueToDeath() override;
	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;
	//~ End ADodCharacter interface

	//~ Begin Pawn interface
	virtual void Reset() override;
	//~ End Pawn interface

	void HandleExperienceLoaded(const UDodExperienceDefinition* CurrentExperience);

	void CheckForAddInitialInventory();
	// 添加库存中的每一个物品，然后激活第一个
	void AddInitialInventory();

	void ClearInventory();

	UFUNCTION()
	void ShowPawnAgainNextFrame(const FActorInitStateChangedParams& Params);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Character")
	TObjectPtr<USkeletalMeshComponent> ArmMesh;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TArray<TSoftClassPtr<UDodInventoryItemDefinition>> InitialInventoryItems;
};
