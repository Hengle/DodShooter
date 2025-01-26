#include "GameMode/GameState/TeamDeathGameState.h"

#include "GameMode/GameState/TeamDeathMatchScoring.h"
#include "Team/DodTeamCreationComponent.h"

ATeamDeathGameState::ATeamDeathGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamDeathMatchScoring = CreateDefaultSubobject<UTeamDeathMatchScoring>(TEXT("TeamDeathMatchScoring"));
	TeamCreation = CreateDefaultSubobject<UDodTeamCreationComponent>(TEXT("TeamCreation"));
}
