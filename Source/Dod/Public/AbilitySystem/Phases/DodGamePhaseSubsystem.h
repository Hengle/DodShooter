#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "DodGamePhaseSubsystem.generated.h"

class UDodGamePhaseAbility;

DECLARE_DELEGATE_OneParam(FDodGamePhaseDelegate, const UDodGamePhaseAbility* Phase);
DECLARE_DELEGATE_OneParam(FDodGamePhaseTagDelegate, const FGameplayTag& PhaseTag);

UENUM(BlueprintType)
enum class EPhaseTagMatchType : uint8
{
	ExactMatch,
	PartialMatch
};

UCLASS()
class DOD_API UDodGamePhaseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void StartPhase(TSubclassOf<UDodGamePhaseAbility> PhaseAbility,
	                FDodGamePhaseDelegate PhaseEndedCallback = FDodGamePhaseDelegate());

	void OnBeginPhase(const UDodGamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle);
	void OnEndPhase(const UDodGamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle);

private:
	struct FDodGamePhaseEntry
	{
		FGameplayTag PhaseTag;
		FDodGamePhaseDelegate PhaseEndedCallback;
	};

	TMap<FGameplayAbilitySpecHandle, FDodGamePhaseEntry> ActivePhaseMap;

	struct FPhaseObserver
	{
	public:
		bool IsMatch(const FGameplayTag& ComparePhaseTag) const;

		FGameplayTag PhaseTag;
		EPhaseTagMatchType MatchType = EPhaseTagMatchType::ExactMatch;
		FDodGamePhaseTagDelegate PhaseCallback;
	};

	TArray<FPhaseObserver> PhaseStartObservers;
	TArray<FPhaseObserver> PhaseEndObservers;
};
