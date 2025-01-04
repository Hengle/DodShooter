#include "AbilitySystem/Abilityies/DodGameplayAbility.h"

#include "AbilitySystemComponent.h"

void UDodGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);

	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && (ActivationPolicy == EDodAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution =
				NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted ||
				NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly;
			const bool bIsServerExecution =
				NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly ||
				NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated;

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UDodGameplayAbility::OnPawnAvatarSet()
{
	K2_OnPawnAvatarSet();
}
