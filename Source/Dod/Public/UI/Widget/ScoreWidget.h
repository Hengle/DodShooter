#pragma once

#include "CoreMinimal.h"
#include "UI/DodTaggedWidget.h"
#include "ScoreWidget.generated.h"

class UDodExperienceDefinition;
class UTeamDeathMatchScoring;
class UCommonNumericTextBlock;
class UCommonTextBlock;

UCLASS()
class DOD_API UScoreWidget : public UDodTaggedWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION()
	void HandleExperienceLoaded(const UDodExperienceDefinition* CurrentExperience);
	
	UFUNCTION()
	void PlayerCounted();

	UFUNCTION()
	void CountDown(float Time);

	UFUNCTION()
	void OnTeamChanged(UObject* ObjectChangingTeam, int32 OldTeamID, int32 NewTeamID);

	void UpdateScore();
	void UpdateClock(float Time);
	void UpdateAllyTeamScore();
	void UpdateEnemyTeamScore();
	void EvaluateSoundRoundEnding();

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* Clock;
	UPROPERTY(meta = (BindWidget))
	UCommonNumericTextBlock* EnemyScore;
	UPROPERTY(meta = (BindWidget))
	UCommonNumericTextBlock* AllyScore;

private:
	UPROPERTY(Transient)
	UTeamDeathMatchScoring* TeamDeathScoring;

	int32 CurrentTeamIndex{INDEX_NONE};
	int32 EnemyTeamIndex{INDEX_NONE};
};
