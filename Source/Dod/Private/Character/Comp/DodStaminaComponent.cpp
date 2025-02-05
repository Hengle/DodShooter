#include "Character/Comp/DodStaminaComponent.h"

#include "AbilitySystem/DodAbilitySystemComponent.h"

UDodStaminaComponent::UDodStaminaComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UDodStaminaComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UDodStaminaComponent::InitializeWithAbilitySystem(UDodAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		return;
	}

	StaminaSet = AbilitySystemComponent->GetSet<UDodStaminaSet>();
	if (!StaminaSet)
	{
		return;
	}
}

void UDodStaminaComponent::UninitializeFromAbilitySystem()
{
}

void UDodStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
}
