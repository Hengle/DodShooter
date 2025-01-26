#include "GameMode/GameState/TeamDeathGameState.h"

#include "GameMode/GameState/TeamDeathMatchScoring.h"

ATeamDeathGameState::ATeamDeathGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamDeathMatchScoring = CreateDefaultSubobject<UTeamDeathMatchScoring>(TEXT("TeamDeathMatchScoring"));
}
