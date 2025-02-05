#include "AbilitySystem/Abilityies/DodGameplayAbility_GaitChange.h"

#include "AbilitySystemComponent.h"
#include "DodGameplayTags.h"

void UDodGameplayAbility_GaitChange::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo,
                                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (ASC->HasMatchingGameplayTag(DodGameplayTags::Movement_Gait_SuperSprint))
	{
	}
	else if (ASC->HasMatchingGameplayTag(DodGameplayTags::Movement_Gait_Sprint))
	{
		ActiveSuperSprint();
	}
	else
	{
		ActiveSprint();
	}
}

void UDodGameplayAbility_GaitChange::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDodGameplayAbility_GaitChange::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDodGameplayAbility_GaitChange::ActiveSprint()
{
	if (SprintAbility)
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
		{
			AbilitySystemComponent->TryActivateAbilityByClass(SprintAbility);
		}
	}
}

void UDodGameplayAbility_GaitChange::ActiveSuperSprint()
{
	if (SuperSprintAbility)
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
		{
			AbilitySystemComponent->TryActivateAbilityByClass(SuperSprintAbility);
		}
	}
}
