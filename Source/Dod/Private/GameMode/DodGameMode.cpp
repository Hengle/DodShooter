#include "GameMode/DodGameMode.h"

#include "Character/DodCharacter.h"
#include "GameMode/GameState/DodGameState.h"
#include "Player/DodPlayerController.h"
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
