#include "GameMode/GameState/TeamDeathMatchScoring.h"

#include "DodGameplayTags.h"
#include "AbilitySystem/Phases/DodGamePhaseSubsystem.h"
#include "AbilitySystem/Phases/DodGamePhase_PostGame.h"
#include "AbilitySystem/Phases/DodGamePhase_Warmup.h"
#include "Character/DodCharacter.h"
#include "GameMode/DodExperienceManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Team/DodTeamSubsystem.h"


UTeamDeathMatchScoring::UTeamDeathMatchScoring()
{
}

void UTeamDeathMatchScoring::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TargetScore);
	DOREPLIFETIME(ThisClass, CountDownTime);
}

bool UTeamDeathMatchScoring::GetTeamScore(int32 InTeamIndex, int32& OutCurrentScore, int32& OutTargetScore)
{
	if (UDodTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UDodTeamSubsystem>())
	{
		OutCurrentScore = TeamSubsystem->GetTeamTagStackCount(InTeamIndex,
		                                                      DodGameplayTags::ShooterGame_Score_Eliminations);
		OutTargetScore = TargetScore;
		return OutCurrentScore >= TargetScore;
	}
	return false;
}

void UTeamDeathMatchScoring::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UDodExperienceManagerComponent* ExperienceComponent =
			GetWorld()->GetGameState()->FindComponentByClass<UDodExperienceManagerComponent>();
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(
			FOnDodExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::HandleExperienceLoaded));
	}
}

void UTeamDeathMatchScoring::HandleExperienceLoaded(const UDodExperienceDefinition* CurrentExperience)
{
	UDodGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UDodGamePhaseSubsystem>();
	GamePhaseSubsystem->StartPhase(UDodGamePhase_Warmup::StaticClass());
	GameStarted();
}

void UTeamDeathMatchScoring::OnEliminationScored()
{
	Super::OnEliminationScored();

	for (int32 TeamIdx = 1; TeamIdx <= 2; ++TeamIdx)
	{
		int32 OutCurrentScore, OutTargetScore;
		if (GetTeamScore(1, OutCurrentScore, OutTargetScore))
		{
			HandleVictory(TeamIdx);
			return;
		}
	}
}

void UTeamDeathMatchScoring::CountDown()
{
	CountDownTime -= 1;
	OnRep_CountDownTime();

	if (CountDownTime > 0)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);

	int32 OutCurrentScore, OutTargetScore;
	int32 TeamAScore = GetTeamScore(1, OutCurrentScore, OutTargetScore);
	int32 TeamBScore = GetTeamScore(2, OutCurrentScore, OutTargetScore);
	if (TeamAScore == TeamBScore)
	{
		HandleVictory(0);
	}
	else if (TeamAScore > TeamBScore)
	{
		HandleVictory(1);
	}
	else
	{
		HandleVictory(2);
	}
}

void UTeamDeathMatchScoring::GameStarted()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADodCharacter::StaticClass(), FoundActors);

	TargetScore = 15;
	OnRep_TargetScore();
	CountDownTime = 120;
	OnRep_CountDownTime();
	GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle, this, &ThisClass::CountDown, 1.f, true);
}

void UTeamDeathMatchScoring::HandleVictory(int32 WinTeamIndex)
{
	if (UDodGamePhaseSubsystem* GamePhaseSubsystem = GetWorld()->GetSubsystem<UDodGamePhaseSubsystem>())
	{
		GamePhaseSubsystem->StartPhase(UDodGamePhase_PostGame::StaticClass());
	}
}

void UTeamDeathMatchScoring::OnRep_TargetScore()
{
	PlayerCounted.Broadcast();
}

void UTeamDeathMatchScoring::OnRep_CountDownTime()
{
	CountDownUpdate.Broadcast(CountDownTime);
}
