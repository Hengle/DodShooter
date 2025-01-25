#include "Player/DodPlayerState.h"

#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DodCombatSet.h"
#include "AbilitySystem/Attributes/DodHealthSet.h"

ADodPlayerState::ADodPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UDodAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthSet = CreateDefaultSubobject<UDodHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UDodCombatSet>(TEXT("CombatSet"));

	NetUpdateFrequency = 100.0f;
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
}