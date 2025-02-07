#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "Team/DodTeamAgentInterface.h"
#include "DodLocalPlayer.generated.h"

UCLASS()
class DOD_API UDodLocalPlayer : public UCommonLocalPlayer, public IDodTeamAgentInterface
{
	GENERATED_BODY()

public:
	//~ Begin UPlayer interface
	virtual void SwitchController(class APlayerController* PC) override;
	//~ End UPlayer interface

	//~ Begin ULocalPlayer interface
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	virtual void InitOnlineSession() override;
	//~ End ULocalPlayer interface

	//~ Begin IDodTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnDodTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~ End IDodTeamAgentInterface interface
protected:
	void OnPlayerControllerChanged(APlayerController* NewController);

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	UPROPERTY()
	FOnDodTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> LastBoundPC;
};
