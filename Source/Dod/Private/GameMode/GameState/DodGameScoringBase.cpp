#include "GameMode/GameState/DodGameScoringBase.h"

#include "DodGameplayTags.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "NativeGameplayTags.h"
#include "Messages/DodVerbMessage.h"
#include "Player/DodPlayerState.h"
#include "Team/DodTeamSubsystem.h"

UDodGameScoringBase::UDodGameScoringBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDodGameScoringBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.RegisterListener(DodGameplayTags::Dod_Elimination_Message, this, &ThisClass::OnReceiveElimination);

	MessageSystem.RegisterListener(DodGameplayTags::Dod_Assist_Message, this, &ThisClass::OnReceiveAssist);
}

void UDodGameScoringBase::OnEliminationScored()
{
}

void UDodGameScoringBase::OnReceiveElimination(FGameplayTag Channel, const FDodVerbMessage& Payload)
{
	if (UDodTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UDodTeamSubsystem>())
	{
		int32 TeamIdA;
		int32 TeamIdB;
		if (TeamSubsystem->CompareTeams(Payload.Instigator, Payload.Target, TeamIdA, TeamIdB)
			== EDodTeamComparison::DifferentTeams)
		{
			TeamSubsystem->AddTeamTagStack(TeamIdA, DodGameplayTags::ShooterGame_Score_Eliminations, 1);
		}
	}
	if (ADodPlayerState* PS = Cast<ADodPlayerState>(Payload.Instigator))
	{
		PS->AddStatTagStack(DodGameplayTags::ShooterGame_Score_Assists, 1);
	}
	if (ADodPlayerState* PS = Cast<ADodPlayerState>(Payload.Target))
	{
		PS->AddStatTagStack(DodGameplayTags::ShooterGame_Score_Deaths, 1);
	}
	OnEliminationScored();
}

void UDodGameScoringBase::OnReceiveAssist(FGameplayTag Channel, const FDodVerbMessage& Payload)
{
	if (ADodPlayerState* PS = Cast<ADodPlayerState>(Payload.Instigator))
	{
		PS->AddStatTagStack(DodGameplayTags::ShooterGame_Score_Assists, 1);
	}
}
