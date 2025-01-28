#include "Dod/Public/GameMode/GameState/DodGameState.h"

#include "AbilitySystemComponent.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameMode/DodActionSet.h"
#include "Messages/DodVerbMessage.h"
#include "Player/DodPlayerSpawningManagerComponent.h"

ADodGameState::ADodGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UDodAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	PlayerSpawningManager = CreateDefaultSubobject<UDodPlayerSpawningManagerComponent>(TEXT("PlayerSpawningManager"));
}

void ADodGameState::BeginPlay()
{
	Super::BeginPlay();

	FGameFeatureActivatingContext Context;

	if (const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld()))
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

void ADodGameState::MulticastReliableMessageToClients_Implementation(const FDodVerbMessage Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

void ADodGameState::MulticastMessageToClients_Implementation(const FDodVerbMessage Message)
{
	MulticastMessageToClients_Implementation(Message);
}
