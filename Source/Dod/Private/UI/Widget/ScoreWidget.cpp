#include "UI/Widget/ScoreWidget.h"

#include "CommonNumericTextBlock.h"
#include "GameFramework/GameStateBase.h"
#include "GameMode/GameState/TeamDeathMatchScoring.h"
#include "Kismet/GameplayStatics.h"
#include "Team/DodTeamAgentInterface.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld()))
	{
		TeamDeathScoring = GameState->GetComponentByClass<UTeamDeathMatchScoring>();
	}
	if (TeamDeathScoring)
	{
		TeamDeathScoring->PlayerCounted.AddDynamic(this, &ThisClass::PlayerCounted);
		TeamDeathScoring->CountDownUpdate.AddDynamic(this, &ThisClass::CountDown);
	}
	UpdateClock(0.f);

	CurrentTeamIndex = INDEX_NONE;
	if (IDodTeamAgentInterface* TeamAgent = Cast<IDodTeamAgentInterface>(GetOwningLocalPlayer()))
	{
		TeamAgent->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnTeamChanged);
		CurrentTeamIndex = GenericTeamIdToInteger(TeamAgent->GetGenericTeamId());
	}
}

void UScoreWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateScore();
}

void UScoreWidget::PlayerCounted()
{
	UpdateScore();
}

void UScoreWidget::CountDown(float Time)
{
	UpdateClock(Time);
}

void UScoreWidget::OnTeamChanged(UObject* ObjectChangingTeam, int32 OldTeamID, int32 NewTeamID)
{
	if (CurrentTeamIndex != INDEX_NONE)
	{
		EnemyTeamIndex = CurrentTeamIndex == 0 ? 0 : CurrentTeamIndex == 1 ? 2 : 1;
	}
	// 更新材质
}

void UScoreWidget::UpdateScore()
{
	if (CurrentTeamIndex > 0)
	{
		UpdateAllyTeamScore();
		UpdateEnemyTeamScore();
		EvaluateSoundRoundEnding();
	}
}

void UScoreWidget::UpdateClock(float Time)
{
	int32 IntTime = Time;
	FString ClockText = FString::Printf(TEXT("%d:%d"), IntTime / 60, IntTime % 60);
	Clock->SetText(FText::FromString(ClockText));
}

void UScoreWidget::UpdateAllyTeamScore()
{
	if (!TeamDeathScoring)
	{
		return;
	}
	int32 OutCurrentScore, OutTargetScore;
	AllyScore->SetCurrentValue(TeamDeathScoring->GetTeamScore(CurrentTeamIndex, OutCurrentScore, OutTargetScore));
}

void UScoreWidget::UpdateEnemyTeamScore()
{
	if (!TeamDeathScoring)
	{
		return;
	}
	int32 OutCurrentScore, OutTargetScore;
	EnemyScore->SetCurrentValue(TeamDeathScoring->GetTeamScore(EnemyTeamIndex, OutCurrentScore, OutTargetScore));
}

void UScoreWidget::EvaluateSoundRoundEnding()
{
}
