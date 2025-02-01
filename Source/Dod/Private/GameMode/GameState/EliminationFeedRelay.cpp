#include "GameMode/GameState/EliminationFeedRelay.h"

#include "DodGameplayTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/GameState/DodGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Messages/DodVerbMessage.h"
#include "Messages/EliminationFeedMessage.h"
#include "Team/DodTeamSubsystem.h"

UEliminationFeedRelay::UEliminationFeedRelay()
{
}

void UEliminationFeedRelay::ReceivedElimination(FGameplayTag Channel, const FDodVerbMessage& Payload)
{
	if (GetOwner()->HasAuthority())
	{
		if (ADodGameState* DodGS = Cast<ADodGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			DodGS->MulticastReliableMessageToClients(Payload);
		}
	}
	if (IsRunningDedicatedServer())
	{
		return;
	}
	UDodTeamSubsystem* TeamSys = GetWorld()->GetSubsystem<UDodTeamSubsystem>();
	if (!TeamSys)
	{
		return;
	}
	APlayerState* AttackerPS = Cast<APlayerState>(Payload.Instigator);
	APlayerState* VictimPS = Cast<APlayerState>(Payload.Target);
	APawn* Attacker = AttackerPS->GetPawn();
	APawn* Victim = VictimPS->GetPawn();
	bool bTemp;
	int32 AttackerTeamId, VictimTeamId;
	TeamSys->FindTeamFromActor(Attacker, bTemp, AttackerTeamId);
	TeamSys->FindTeamFromActor(Victim, bTemp, VictimTeamId);

	FEliminationFeedMessage Message;
	Message.Attacker = FText::FromString(AttackerPS->GetPlayerName());
	Message.Victim = FText::FromString(VictimPS->GetPlayerName());
	Message.TargetChannel = FGameplayTag::RequestGameplayTag(TEXT("HUD.Slot.EliminationFeed"));
	Message.AttackerTeamId = AttackerTeamId;
	Message.VictimTeamId = VictimTeamId;
	Message.InstigatorTags = Payload.InstigatorTags;

	UGameplayMessageSubsystem::Get(GetWorld()).
		BroadcastMessage(DodGameplayTags::Dod_AddNotification_KillFeed, Message);
}

void UEliminationFeedRelay::BeginPlay()
{
	Super::BeginPlay();

	UGameplayMessageSubsystem::Get(GetWorld()).RegisterListener(DodGameplayTags::Dod_Elimination_Message, this,
	                                                            &ThisClass::ReceivedElimination);
}
