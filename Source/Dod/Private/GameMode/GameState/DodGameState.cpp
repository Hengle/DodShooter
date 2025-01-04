#include "Dod/Public/GameMode/GameState/DodGameState.h"

#include "AbilitySystemComponent.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "GameMode/DodActionSet.h"

ADodGameState::ADodGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UDodAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void ADodGameState::BeginPlay()
{
	Super::BeginPlay();

	FGameFeatureActivatingContext Context;

	const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
	if (ExistingWorldContext)
	{
		Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
	}

	auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
	{
		for (UGameFeatureAction* Action : ActionList)
		{
			if (Action != nullptr)
			{
				Action->OnGameFeatureRegistering();
				Action->OnGameFeatureLoading();
				Action->OnGameFeatureActivating(Context);
			}
		}
	};

	for (const TObjectPtr<UDodActionSet>& ActionSet : ActionSets)
	{
		if (ActionSet != nullptr)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}
}

void ADodGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* ADodGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
