#include "Dod/Public/GameMode/GameState/DodGameState.h"

#include "AbilitySystemComponent.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameMode/DodActionSet.h"
#include "GameMode/DodExperienceManagerComponent.h"
#include "Messages/DodVerbMessage.h"
#include "Net/UnrealNetwork.h"
#include "Player/DodPlayerSpawningManagerComponent.h"

extern ENGINE_API float GAverageFPS;

ADodGameState::ADodGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = CreateDefaultSubobject<UDodAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ExperienceManagerComponent =
		CreateDefaultSubobject<UDodExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
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

void ADodGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerFPS);
}

void ADodGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() == ROLE_Authority)
	{
		ServerFPS = GAverageFPS;
	}
}

UAbilitySystemComponent* ADodGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ADodGameState::GetServerFPS() const
{
	return ServerFPS;
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
