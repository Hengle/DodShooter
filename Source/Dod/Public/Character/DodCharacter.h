#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularCharacter.h"
#include "DodCharacter.generated.h"

class UDodAbilitySystemComponent;
class UAbilitySystemComponent;
class USpringArmComponent;
class UDodPawnExtensionComponent;
class UDodStaminaComponent;
class UDodCameraComponent;
class UDodHealthComponent;

UCLASS(Blueprintable)
class DOD_API ADodCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADodCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Dod|Character")
	UDodAbilitySystemComponent* GetDodAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//~ Begin Actor interface
	virtual void BeginPlay() override;
	//~ End of Actor interface

protected:
	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void InitializeGameplayTags();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Character")
	TObjectPtr<USkeletalMeshComponent> HeadMesh;

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
