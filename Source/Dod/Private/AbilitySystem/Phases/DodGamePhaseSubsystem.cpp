#include "AbilitySystem/Phases/DodGamePhaseSubsystem.h"

#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "AbilitySystem/Phases/DodGamePhaseAbility.h"
#include "GameFramework/GameStateBase.h"

struct FGameplayAbilitySpec;

void UDodGamePhaseSubsystem::StartPhase(TSubclassOf<UDodGamePhaseAbility> PhaseAbility,
                                        FDodGamePhaseDelegate PhaseEndedCallback)
{
	UWorld* World = GetWorld();
	UDodAbilitySystemComponent* GameState_ASC =
		World->GetGameState()->FindComponentByClass<UDodAbilitySystemComponent>();
	if (ensure(GameState_ASC))
	{
		FGameplayAbilitySpec PhaseSpec(PhaseAbility, 1, 0, this);
		FGameplayAbilitySpecHandle SpecHandle = GameState_ASC->GiveAbilityAndActivateOnce(PhaseSpec);
		FGameplayAbilitySpec* FoundSpec = GameState_ASC->FindAbilitySpecFromHandle(SpecHandle);

		if (FoundSpec && FoundSpec->IsActive())
		{
			FDodGamePhaseEntry& Entry = ActivePhaseMap.FindOrAdd(SpecHandle);
			Entry.PhaseEndedCallback = PhaseEndedCallback;
		}
		else
		{
			PhaseEndedCallback.ExecuteIfBound(nullptr);
		}
	}
}

void UDodGamePhaseSubsystem::OnBeginPhase(const UDodGamePhaseAbility* PhaseAbility,
                                          const FGameplayAbilitySpecHandle PhaseAbilityHandle)
{
	const FGameplayTag IncomingPhaseTag = PhaseAbility->GetGamePhaseTag();

	const UWorld* World = GetWorld();
	UDodAbilitySystemComponent* GameState_ASC =
		World->GetGameState()->FindComponentByClass<UDodAbilitySystemComponent>();
	if (ensure(GameState_ASC))
	{
		TArray<FGameplayAbilitySpec*> ActivePhases;
		for (const auto& KVP : ActivePhaseMap)
		{
			const FGameplayAbilitySpecHandle ActiveAbilityHandle = KVP.Key;
			if (FGameplayAbilitySpec* Spec = GameState_ASC->FindAbilitySpecFromHandle(ActiveAbilityHandle))
			{
				ActivePhases.Add(Spec);
			}
		}

		for (const FGameplayAbilitySpec* ActivePhase : ActivePhases)
		{
			const UDodGamePhaseAbility* ActivePhaseAbility = CastChecked<UDodGamePhaseAbility>(ActivePhase->Ability);
			const FGameplayTag ActivePhaseTag = ActivePhaseAbility->GetGamePhaseTag();

			if (!IncomingPhaseTag.MatchesTag(ActivePhaseTag))
			{
				FGameplayAbilitySpecHandle HandleToEnd = ActivePhase->Handle;
				GameState_ASC->CancelAbilitiesByFunc(
					[HandleToEnd](const UDodGameplayAbility* DodAbility, FGameplayAbilitySpecHandle Handle)
					{
						return Handle == HandleToEnd;
					}, true);
			}
		}

		FDodGamePhaseEntry& Entry = ActivePhaseMap.FindOrAdd(PhaseAbilityHandle);
		Entry.PhaseTag = IncomingPhaseTag;

		for (const FPhaseObserver& Observer : PhaseStartObservers)
		{
			if (Observer.IsMatch(IncomingPhaseTag))
			{
				Observer.PhaseCallback.ExecuteIfBound(IncomingPhaseTag);
			}
		}
	}
}

void UDodGamePhaseSubsystem::OnEndPhase(const UDodGamePhaseAbility* PhaseAbility,
                                        const FGameplayAbilitySpecHandle PhaseAbilityHandle)
{
	const FGameplayTag EndedPhaseTag = PhaseAbility->GetGamePhaseTag();

	const FDodGamePhaseEntry& Entry = ActivePhaseMap.FindChecked(PhaseAbilityHandle);
	Entry.PhaseEndedCallback.ExecuteIfBound(PhaseAbility);

	ActivePhaseMap.Remove(PhaseAbilityHandle);

	for (const FPhaseObserver& Observer : PhaseEndObservers)
	{
		if (Observer.IsMatch(EndedPhaseTag))
		{
			Observer.PhaseCallback.ExecuteIfBound(EndedPhaseTag);
		}
	}
}

bool UDodGamePhaseSubsystem::FPhaseObserver::IsMatch(const FGameplayTag& ComparePhaseTag) const
{
	switch (MatchType)
	{
	case EPhaseTagMatchType::ExactMatch:
		return ComparePhaseTag == PhaseTag;
	case EPhaseTagMatchType::PartialMatch:
		return ComparePhaseTag.MatchesTag(PhaseTag);
	}

	return false;
}
