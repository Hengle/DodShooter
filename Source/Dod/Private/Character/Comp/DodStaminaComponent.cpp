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

	StaminaSet->OnStaminaChanged.AddUObject(this, &ThisClass::HandleStaminaChanged);
	StaminaSet->OnMaxStaminaChanged.AddUObject(this, &ThisClass::HandleMaxStaminaChanged);

	AbilitySystemComponent->SetNumericAttributeBase(UDodStaminaSet::GetStaminaAttribute(), StaminaSet->GetMaxStamina());

	OnStaminaChanged.Broadcast(this, StaminaSet->GetStamina(), StaminaSet->GetStamina(), nullptr);
	OnMaxStaminaChanged.Broadcast(this, StaminaSet->GetStamina(), StaminaSet->GetStamina(), nullptr);
}

void UDodStaminaComponent::UninitializeFromAbilitySystem()
{
	if (StaminaSet)
	{
		StaminaSet->OnStaminaChanged.RemoveAll(this);
		StaminaSet->OnMaxStaminaChanged.RemoveAll(this);
	}
}

float UDodStaminaComponent::GetStamina() const
{
	return StaminaSet ? StaminaSet->GetStamina() : 0.0f;
}

float UDodStaminaComponent::GetMaxStamina() const
{
	return StaminaSet ? StaminaSet->GetMaxStamina() : 0.0f;
}

void UDodStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDodStaminaComponent::HandleStaminaChanged(AActor* DamageInstigator, AActor* DamageCauser,
                                                const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude,
                                                float OldValue, float NewValue)
{
	OnStaminaChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UDodStaminaComponent::HandleMaxStaminaChanged(AActor* DamageInstigator, AActor* DamageCauser,
                                                   const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude,
                                                   float OldValue, float NewValue)
{
	OnMaxStaminaChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}
