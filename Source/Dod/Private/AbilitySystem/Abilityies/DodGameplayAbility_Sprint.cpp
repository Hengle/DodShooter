#include "AbilitySystem/Abilityies/DodGameplayAbility_Sprint.h"

#include "AbilitySystemComponent.h"
#include "DodGameplayTags.h"
#include "Character/DodCharacter.h"
#include "Character/DodCharacterMovementComp.h"

void UDodGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		AbilitySystemComponent->AddLooseGameplayTag(AddTagToPlayer);
	}

	GetWorld()->GetTimerManager().SetTimer(
		StaminaCostTimerHandle,
		this, &ThisClass::OnStaminaCostTick,
		.05f,
		true);
}

void UDodGameplayAbility_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            bool bReplicateEndAbility, bool bWasCancelled)
{
	if (StaminaCostTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaCostTimerHandle);
	}
	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(AddTagToPlayer);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDodGameplayAbility_Sprint::OnStaminaCostTick()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	FScopedPredictionWindow ScopedPrediction(ASC, true);

	if (ADodCharacter* Character = GetDodCharacterFromActorInfo())
	{
		if (UDodCharacterMovementComp* MC = Cast<UDodCharacterMovementComp>(Character->GetMovementComponent()))
		{
			if (MC->GetCurrentAcceleration().Size() > 10.f && !MC->IsFalling())
			{
				if (CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
				{
					return;
				}
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
