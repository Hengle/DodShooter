#include "Character/Comp/DodHealthComponent.h"

#include "DodGameplayTags.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DodHealthSet.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messages/DodVerbMessage.h"
#include "Messages/DodVerbMessageHelpers.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "System/DodAssetManager.h"
#include "System/DodGameData.h"

UDodHealthComponent::UDodHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UDodHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDodHealthComponent, DeathState);
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

void UDodHealthComponent::StartDeath()
{
	if (DeathState != EDodDeathState::NotDead)
	{
		return;
	}

	DeathState = EDodDeathState::DeathStarted;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(DodGameplayTags::Status_Death_Dying, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathStarted.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void UDodHealthComponent::FinishDeath()
{
	if (DeathState != EDodDeathState::DeathStarted)
	{
		return;
	}

	DeathState = EDodDeathState::DeathFinished;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(DodGameplayTags::Status_Death_Dead, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathFinished.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void UDodHealthComponent::DamageSelfDestruct(bool bFellOutOfWorld)
{
	if (DeathState == EDodDeathState::NotDead && AbilitySystemComponent)
	{
		const TSubclassOf<UGameplayEffect> DamageGE = UDodAssetManager::GetSubclass(
			UDodGameData::Get().DamageGameplayEffect_SetByCaller);
		if (!DamageGE)
		{
			return;
		}

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			DamageGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		if (!Spec)
		{
			return;
		}

		Spec->AddDynamicAssetTag(TAG_Gameplay_DamageSelfDestruct);

		if (bFellOutOfWorld)
		{
			Spec->AddDynamicAssetTag(TAG_Gameplay_FellOutOfWorld);
		}

		const float DamageAmount = GetMaxHealth();

		Spec->SetSetByCallerMagnitude(DodGameplayTags::SetByCaller_Damage, DamageAmount);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

void UDodHealthComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UDodHealthComponent::ClearGameplayTags()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(DodGameplayTags::Status_Death_Dying, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(DodGameplayTags::Status_Death_Dead, 0);
	}
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
			Message.Verb = DodGameplayTags::Dod_Elimination_Message;
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

void UDodHealthComponent::OnRep_DeathState(EDodDeathState OldDeathState)
{
	const EDodDeathState NewDeathState = DeathState;

	DeathState = OldDeathState;

	if (OldDeathState > NewDeathState)
	{
		return;
	}

	if (OldDeathState == EDodDeathState::NotDead)
	{
		if (NewDeathState == EDodDeathState::DeathStarted)
		{
			StartDeath();
		}
		else if (NewDeathState == EDodDeathState::DeathFinished)
		{
			StartDeath();
			FinishDeath();
		}
	}
	else if (OldDeathState == EDodDeathState::DeathStarted)
	{
		if (NewDeathState == EDodDeathState::DeathFinished)
		{
			FinishDeath();
		}
	}
}
