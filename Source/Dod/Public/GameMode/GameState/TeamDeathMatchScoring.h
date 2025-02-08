#pragma once

#include "CoreMinimal.h"
#include "DodGameScoringBase.h"
#include "TeamDeathMatchScoring.generated.h"

class UDodExperienceDefinition;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerCounted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountDownUpdate, float, Time);

UCLASS(meta=(BlueprintSpawnableComponent))
class DOD_API UTeamDeathMatchScoring : public UDodGameScoringBase
{
	GENERATED_BODY()

public:
	UTeamDeathMatchScoring();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	bool GetTeamScore(int32 InTeamIndex, int32& OutCurrentScore, int32& OutTargetScore);

	FPlayerCounted PlayerCounted;

	FCountDownUpdate CountDownUpdate;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void HandleExperienceLoaded(const UDodExperienceDefinition* CurrentExperience);

	//~ Begin UDodGameScoringBase interface
	virtual void OnEliminationScored() override;
	//~ End of UDodGameScoringBase interface

	void CountDown();

	void GameStarted();

	void HandleVictory(int32 WinTeamIndex);

	UFUNCTION()
	void OnRep_TargetScore();
	UFUNCTION()
	void OnRep_CountDownTime();

	UPROPERTY(Replicated=OnRep_TargetScore)
	float TargetScore{0.f};
	UPROPERTY(Replicated=OnRep_CountDownTime)
	float CountDownTime{10.f};

	FTimerHandle CountDownTimerHandle;
};
