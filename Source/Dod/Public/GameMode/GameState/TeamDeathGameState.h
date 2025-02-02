#pragma once

#include "CoreMinimal.h"
#include "DodGameState.h"
#include "TeamDeathGameState.generated.h"

class UDodBotCreationComponent;
class UEliminationFeedRelay;
class UDodTeamCreationComponent;
class UTeamDeathMatchScoring;

UCLASS()
class DOD_API ATeamDeathGameState : public ADodGameState
{
	GENERATED_BODY()

public:
	ATeamDeathGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTeamDeathMatchScoring> TeamDeathMatchScoring;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UDodTeamCreationComponent> TeamCreation;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UEliminationFeedRelay> MessageProcessor;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UDodBotCreationComponent> BotCreationComp{nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UDodBotCreationComponent> BotCreationCompClass;
};
