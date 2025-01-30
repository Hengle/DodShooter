#include "AbilitySystem/Abilityies/DodGameplayAbility_Death.h"

#include "DodGameplayTags.h"
#include "DodLogChannels.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "Character/Comp/DodHealthComponent.h"

UDodGameplayAbility_Death::UDodGameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	bAutoStartDeath = true;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = DodGameplayTags::GameplayEvent_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
	AbilityTags.AddTag(DodGameplayTags::Ability_Type_StatusChange_Death);
}

void UDodGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	UDodAbilitySystemComponent* LyraASC = CastChecked<UDodAbilitySystemComponent>(
		ActorInfo->AbilitySystemComponent.Get());

	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(DodGameplayTags::Ability_Behavior_SurvivesDeath);

	LyraASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);

	SetCanBeCanceled(false);

	if (!ChangeActivationGroup(EDodAbilityActivationGroup::Exclusive_Blocking))
	{
		UE_LOG(LogDod, Error,
		       TEXT(
			       "ULyraGameplayAbility_Death::ActivateAbility: Ability [%s] failed to change activation group to blocking."
		       ), *GetName());
	}

	if (bAutoStartDeath)
	{
		StartDeath();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FTimerHandle DeathTimer;
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, FTimerDelegate::CreateLambda([&]
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo
		           , CurrentActivationInfo, true, true);
	}), 8.f, false);
}

void UDodGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);
	FinishDeath();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDodGameplayAbility_Death::StartDeath()
{
	if (UDodHealthComponent* HealthComponent = UDodHealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == EDodDeathState::NotDead)
		{
			HealthComponent->StartDeath();
		}
	}
}

void UDodGameplayAbility_Death::FinishDeath()
{
	if (UDodHealthComponent* HealthComponent = UDodHealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == EDodDeathState::DeathStarted)
		{
			HealthComponent->FinishDeath();
		}
	}
}
