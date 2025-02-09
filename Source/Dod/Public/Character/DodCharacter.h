#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "ModularCharacter.h"
#include "Team/DodTeamAgentInterface.h"
#include "DodCharacter.generated.h"

class UDodAbilitySystemComponent;
class UAbilitySystemComponent;
class USpringArmComponent;
class UDodPawnExtensionComponent;
class UDodStaminaComponent;
class UDodCameraComponent;
class UDodHealthComponent;

UCLASS(Blueprintable)
class DOD_API ADodCharacter : public AModularCharacter, public IAbilitySystemInterface, public IDodTeamAgentInterface
{
	GENERATED_BODY()

public:
	ADodCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Dod|Character")
	UDodAbilitySystemComponent* GetDodAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void ToggleCrouch();

	//~ Begin Actor interface
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End Actor interface

	//~ Begin ACharacter interface
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	//~ End ACharacter interface

	//~ Begin IDodTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnDodTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~ End IDodTeamAgentInterface interface

	void AddControllerRotation(FRotator InRotator);

protected:
	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void InitializeGameplayTags();

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	void DisableMovementAndCollision();
	virtual void DestroyDueToDeath();
	void UninitAndDestroy();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);

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

private:
	UPROPERTY(ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY()
	FOnDodTeamIndexChangedDelegate OnTeamChangedDelegate;
};
