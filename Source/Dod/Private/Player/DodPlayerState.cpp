#include "Player/DodPlayerState.h"

#include "AbilitySystem/DodAbilitySet.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DodCombatSet.h"
#include "AbilitySystem/Attributes/DodHealthSet.h"
#include "AbilitySystem/Attributes/DodStaminaSet.h"
#include "Net/UnrealNetwork.h"

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

void ADodPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	TArray<FDodAbilitySet_GrantedHandles> AbilitySetHandles;

	for (const TSoftObjectPtr<const UDodAbilitySet>& SetPtr : GrantedAbilitySets)
	{
		if (const UDodAbilitySet* Set = SetPtr.LoadSynchronous())
		{
			Set->GiveToAbilitySystem(AbilitySystemComponent, &AbilitySetHandles.AddDefaulted_GetRef());
		}
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

void ADodPlayerState::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void ADodPlayerState::OnRep_MySquadID()
{
}
