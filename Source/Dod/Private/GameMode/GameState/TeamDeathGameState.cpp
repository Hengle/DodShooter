#include "GameMode/GameState/TeamDeathGameState.h"

#include "GameMode/DodBotCreationComponent.h"
#include "GameMode/GameState/EliminationFeedRelay.h"
#include "GameMode/GameState/TeamDeathMatchScoring.h"
#include "Messages/GameplayMessageProcessor.h"
#include "Team/DodTeamCreationComponent.h"

ATeamDeathGameState::ATeamDeathGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ATeamDeathGameState::BeginPlay()
{
	Super::BeginPlay();
}
