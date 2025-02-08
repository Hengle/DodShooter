#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DodGameMode.generated.h"

class UDodPawnData;
enum class ECommonUserOnlineContext : uint8;
class UDodExperienceDefinition;
class UCommonUserInfo;
enum class ECommonSessionOnlineMode : uint8;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDodGameModePlayerInitialized, AGameModeBase*, AController*);

UCLASS()
class DOD_API ADodGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADodGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Dod|Pawn")
	const UDodPawnData* GetPawnDataForController(const AController* InController) const;

	//~ Begin AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	                                                          const FTransform& SpawnTransform) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	virtual void InitGameState() override;
	virtual bool UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage) override;
	virtual void GenericPlayerInitialization(AController* C) override;
	virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	//~ End AGameModeBase interface

	virtual bool ControllerCanRestart(AController* Controller);

	UFUNCTION(BlueprintCallable)
	void RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset = false);

protected:
	void OnExperienceLoaded(const UDodExperienceDefinition* CurrentExperience);
	bool IsExperienceLoaded() const;

	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);

	bool TryDedicatedServerLogin();
	void HostDedicatedServerMatch(ECommonSessionOnlineMode OnlineMode);

	UFUNCTION()
	void OnUserInitializedForDedicatedServer(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error,
	                                         ECommonUserPrivilege RequestedPrivilege,
	                                         ECommonUserOnlineContext OnlineContext);

public:
	FOnDodGameModePlayerInitialized OnGameModePlayerInitialized;
};
