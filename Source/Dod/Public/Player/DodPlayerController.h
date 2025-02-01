#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "Team/DodTeamAgentInterface.h"
#include "DodPlayerController.generated.h"

class UDodWeaponStateComponent;
class ADodPlayerState;
class UDodAbilitySystemComponent;
class UDodQuickBarComponent;
class UDodInventoryManagerComponent;

UCLASS()
class DOD_API ADodPlayerController : public ACommonPlayerController, public IDodTeamAgentInterface
{
	GENERATED_BODY()

public:
	ADodPlayerController(const FObjectInitializer& FObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Dod|PlayerController")
	ADodPlayerState* GetDodPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Dod|PlayerController")
	UDodAbilitySystemComponent* GetDodAbilitySystemComponent() const;

	//~ Begin AController interface
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~ End of AController interface

	//~ Begin APlayerController interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~ End of APlayerController interface

	//~ Begin IDodTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnDodTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~ End of IDodTeamAgentInterface

	void UnEquip();
private:
	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);
	
	void BroadcastOnPlayerStateChanged();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TObjectPtr<UDodInventoryManagerComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TObjectPtr<UDodQuickBarComponent> QuickBarComponent;

	UPROPERTY()
	TObjectPtr<UDodWeaponStateComponent> WeaponStateComponent;

private:
	UPROPERTY()
	FOnDodTeamIndexChangedDelegate OnTeamChangedDelegate;
	
	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;
};
