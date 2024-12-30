#include "CommonPlayerController.h"

#include "CommonLocalPlayer.h"

ACommonPlayerController::ACommonPlayerController(const FObjectInitializer& ObjectInitializer)
{
}

void ACommonPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	
	if (UCommonLocalPlayer* LocalPlayer = Cast<UCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);

		if (PlayerState)
		{
			LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);
		}
	}
}
