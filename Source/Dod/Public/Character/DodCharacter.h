#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "DodCharacter.generated.h"

class USpringArmComponent;
class UDodPawnExtensionComponent;
class UDodStaminaComponent;
class UDodCameraComponent;
class UDodHealthComponent;

UCLASS(Blueprintable)
class DOD_API ADodCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	ADodCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin Actor interface
	virtual void BeginPlay() override;
	//~ End of Actor interface

	void ChangeToFirstPerson();
	void ChangeToThirdPerson();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Character")
	USkeletalMeshComponent* ArmMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Character")
	USkeletalMeshComponent* HeadMesh;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dod|Character")
	TObjectPtr<UDodPawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dod|Character")
	TObjectPtr<UDodHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dod|Character")
	TObjectPtr<UDodStaminaComponent> StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dod|Character")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dod|Character")
	TObjectPtr<UDodCameraComponent> CameraComponent;
};
