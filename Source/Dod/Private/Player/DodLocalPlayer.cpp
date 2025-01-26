#include "Player/DodLocalPlayer.h"

void UDodLocalPlayer::SwitchController(class APlayerController* PC)
{
	Super::SwitchController(PC);

	OnPlayerControllerChanged(PlayerController);
}

bool UDodLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);
	OnPlayerControllerChanged(PlayerController);
	return bResult;
}

void UDodLocalPlayer::InitOnlineSession()
{
	OnPlayerControllerChanged(PlayerController);
	Super::InitOnlineSession();
}

void UDodLocalPlayer::SetGenericTeamId(const FGenericTeamId& TeamID)
{
}

FGenericTeamId UDodLocalPlayer::GetGenericTeamId() const
{
	if (IDodTeamAgentInterface* ControllerAsTeamProvider = Cast<IDodTeamAgentInterface>(PlayerController))
	{
		return ControllerAsTeamProvider->GetGenericTeamId();
	}
	else
	{
		return FGenericTeamId::NoTeam;
	}
}

FOnDodTeamIndexChangedDelegate* UDodLocalPlayer::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void UDodLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
{
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (IDodTeamAgentInterface* ControllerAsTeamProvider = Cast<IDodTeamAgentInterface>(LastBoundPC.Get()))
	{
		OldTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
	}

	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (IDodTeamAgentInterface* ControllerAsTeamProvider = Cast<IDodTeamAgentInterface>(NewController))
	{
		NewTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
		LastBoundPC = NewController;
	}

	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
}

void UDodLocalPlayer::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}
