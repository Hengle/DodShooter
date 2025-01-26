#include "Character/Comp/DodHealthComponent.h"

#include "DodGameplayTags.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DodHealthSet.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messages/DodVerbMessage.h"
#include "Messages/DodVerbMessageHelpers.h"
#include "GameFramework/PlayerState.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Dod_Elimination_Message, "Dod.Elimination.Message");

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
#if WITH_SERVER_CODE
	if (AbilitySystemComponent && DamageEffectSpec)
	{
		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		{
			FGameplayEventData Payload;
			Payload.EventTag = DodGameplayTags::GameplayEvent_Death;
			Payload.Instigator = DamageInstigator;
			Payload.Target = AbilitySystemComponent->GetAvatarActor();
			Payload.OptionalObject = DamageEffectSpec->Def;
			Payload.ContextHandle = DamageEffectSpec->GetEffectContext();
			Payload.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
			Payload.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
			Payload.EventMagnitude = DamageMagnitude;

			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}

		// Send a standardized verb message that other systems can observe
		{
			FDodVerbMessage Message;
			Message.Verb = TAG_Dod_Elimination_Message;
			Message.Instigator = DamageInstigator;
			Message.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
			Message.Target =
				UDodVerbMessageHelpers::GetPlayerStateFromObject(AbilitySystemComponent->GetAvatarActor());
			Message.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
			//@TODO: Fill out context tags, and any non-ability-system source/instigator tags
			//@TODO: Determine if it's an opposing team kill, self-own, team kill, etc...

			UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageSystem.BroadcastMessage(Message.Verb, Message);
		}

		//@TODO: assist messages (could compute from damage dealt elsewhere)?
	}

#endif // #if WITH_SERVER_CODE
}
