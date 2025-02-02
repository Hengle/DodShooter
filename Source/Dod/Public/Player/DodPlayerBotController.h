#pragma once

#include "CoreMinimal.h"
#include "ModularAIController.h"
#include "Team/DodTeamAgentInterface.h"
#include "DodPlayerBotController.generated.h"

UCLASS()
class DOD_API ADodPlayerBotController : public AModularAIController, public IDodTeamAgentInterface
{
	GENERATED_BODY()

public:
	ADodPlayerBotController();

	//~ Begin IDodTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnDodTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End of IDodTeamAgentInterface interface


	//~ Begin AController interface
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~ End of AController interface

	void ServerRestartController();

protected:
	virtual void BeginPlay() override;

	void BroadcastOnPlayerStateChanged();

	virtual void OnPlayerStateChanged();

	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	UPROPERTY()
	FOnDodTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;
};
