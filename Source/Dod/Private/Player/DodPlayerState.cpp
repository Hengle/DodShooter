#include "Player/DodPlayerState.h"

#include "DodLogChannels.h"
#include "AbilitySystem/DodAbilitySet.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DodCombatSet.h"
#include "AbilitySystem/Attributes/DodHealthSet.h"
#include "AbilitySystem/Attributes/DodStaminaSet.h"
#include "Character/DodPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameMode/DodExperienceManagerComponent.h"
#include "GameMode/DodGameMode.h"
#include "Net/UnrealNetwork.h"

const FName ADodPlayerState::NAME_DodAbilityReady("DodAbilitiesReady");

ADodPlayerState::ADodPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UDodAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthSet = CreateDefaultSubobject<UDodHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UDodCombatSet>(TEXT("CombatSet"));
	StaminaSet = CreateDefaultSubobject<UDodStaminaSet>(TEXT("StaminaSet"));

	NetUpdateFrequency = 100.0f;

	MyTeamID = FGenericTeamId::NoTeam;
	MySquadID = INDEX_NONE;
}

void ADodPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MySquadID, SharedParams);

	SharedParams.Condition = ELifetimeCondition::COND_SkipOwner;

	DOREPLIFETIME(ThisClass, StatTags);
}

ADodPlayerState* ADodPlayerState::GetDodPlayerController() const
{
	return Cast<ADodPlayerState>(GetOwner());
}

UAbilitySystemComponent* ADodPlayerState::GetAbilitySystemComponent() const
{
	return GetDodAbilitySystemComponent();
}

void ADodPlayerState::SetPawnData(const UDodPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogDod, Error,
		       TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."),
		       *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const UDodAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_DodAbilityReady);

	ForceNetUpdate();
}

void ADodPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	TArray<FDodAbilitySet_GrantedHandles> AbilitySetHandles;

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		UDodExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<
			UDodExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(
			FOnDodExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void ADodPlayerState::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamID = MyTeamID;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
		MyTeamID = InTeamID;
		ConditionalBroadcastTeamChanged(this, OldTeamID, InTeamID);
	}
}

FGenericTeamId ADodPlayerState::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnDodTeamIndexChangedDelegate* ADodPlayerState::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void ADodPlayerState::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void ADodPlayerState::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 ADodPlayerState::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool ADodPlayerState::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void ADodPlayerState::OnExperienceLoaded(const UDodExperienceDefinition* CurrentExperience)
{
	if (ADodGameMode* DodGameMode = GetWorld()->GetAuthGameMode<ADodGameMode>())
	{
		if (const UDodPawnData* NewPawnData = DodGameMode->GetPawnDataForController(GetOwningController()))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogDod, Error,
			       TEXT(
				       "ADodPlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"
			       ), *GetNameSafe(this));
		}
	}
}

void ADodPlayerState::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void ADodPlayerState::OnRep_MySquadID()
{
}

void ADodPlayerState::OnRep_PawnData()
{
}
