#include "Character/Comp/DodHealthComponent.h"

#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DodHealthSet.h"


UDodHealthComponent::UDodHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UDodHealthComponent::InitializeWithAbilitySystem(UDodAbilitySystemComponent* InASC)
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

	HealthSet = AbilitySystemComponent->GetSet<UDodHealthSet>();
	if (!HealthSet)
	{
		return;
	}

	HealthSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

	AbilitySystemComponent->SetNumericAttributeBase(UDodHealthSet::GetHealthAttribute(), HealthSet->GetMaxHealth());

	ClearGameplayTags();

	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
}

void UDodHealthComponent::UninitializeFromAbilitySystem()
{
	ClearGameplayTags();

	if (HealthSet)
	{
		HealthSet->OnHealthChanged.RemoveAll(this);
		HealthSet->OnMaxHealthChanged.RemoveAll(this);
		HealthSet->OnOutOfHealth.RemoveAll(this);
	}

	HealthSet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UDodHealthComponent::GetHealth() const
{
	return HealthSet ? HealthSet->GetHealth() : 0.0f;
}

float UDodHealthComponent::GetMaxHealth() const
{
	return HealthSet ? HealthSet->GetMaxHealth() : 0.0f;
}

float UDodHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();

		return MaxHealth > 0.0f ? Health / MaxHealth : 0.0f;
	}

	return 0.0f;
}

void UDodHealthComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UDodHealthComponent::ClearGameplayTags()
{
}

void UDodHealthComponent::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser,
                                              const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude,
                                              float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UDodHealthComponent::HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser,
                                                 const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude,
                                                 float OldValue, float NewValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UDodHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser,
                                            const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude,
                                            float OldValue, float NewValue)
{
}
