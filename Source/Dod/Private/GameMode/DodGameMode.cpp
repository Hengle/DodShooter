#include "GameMode/DodGameMode.h"

#include "Character/DodCharacter.h"
#include "Character/Comp/DodPawnExtensionComponent.h"
#include "GameMode/GameState/DodGameState.h"
#include "Player/DodPlayerBotController.h"
#include "Player/DodPlayerController.h"
#include "Player/DodPlayerSpawningManagerComponent.h"
#include "Player/DodPlayerState.h"
#include "UI/DodHUD.h"

ADodGameMode::ADodGameMode(const FObjectInitializer& ObjectInitializer)
{
	GameStateClass = ADodGameState::StaticClass();
	PlayerControllerClass = ADodPlayerController::StaticClass();
	PlayerStateClass = ADodPlayerState::StaticClass();
	DefaultPawnClass = ADodCharacter::StaticClass();
	HUDClass = ADodHUD::StaticClass();
}

void ADodGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

APawn* ADodGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
                                                                const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
	}
	return nullptr;
}

void ADodGameMode::GenericPlayerInitialization(AController* C)
{
	Super::GenericPlayerInitialization(C);

	OnGameModePlayerInitialized.Broadcast(this, C);
}

bool ADodGameMode::ControllerCanRestart(AController* Controller)
{
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{	
		if (!Super::PlayerCanRestart_Implementation(PC))
		{
			return false;
		}
	}
	else
	{
		// Bot version of Super::PlayerCanRestart_Implementation
		if ((Controller == nullptr) || Controller->IsPendingKillPending())
		{
			return false;
		}
	}

	if (UDodPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UDodPlayerSpawningManagerComponent>())
	{
		return PlayerSpawningComponent->ControllerCanRestart(Controller);
	}

	return true;
}

void ADodGameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if (bForceReset && (Controller != nullptr))
	{
		Controller->Reset();
	}

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PC, &APlayerController::ServerRestartPlayer_Implementation);
	}
	else if (ADodPlayerBotController* BotController = Cast<ADodPlayerBotController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(BotController, &ADodPlayerBotController::ServerRestartController);
	}
}
