#pragma once

#include "CoreMinimal.h"
#include "DodGameState.h"
#include "TeamDeathGameState.generated.h"

class UDodTeamCreationComponent;
class UTeamDeathMatchScoring;

UCLASS()
class DOD_API ATeamDeathGameState : public ADodGameState
{
	GENERATED_BODY()
public:
	ATeamDeathGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTeamDeathMatchScoring> TeamDeathMatchScoring;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UDodTeamCreationComponent> TeamCreation;
};
