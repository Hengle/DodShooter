#include "Player/DodPlayerState.h"

#include "AbilitySystem/DodAbilitySystemComponent.h"

ADodPlayerState::ADodPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UDodAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

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
