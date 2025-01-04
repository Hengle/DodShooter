#include "AbilitySystem/DodAbilitySystemComponent.h"

#include "DodGlobalAbilitySystem.h"
#include "AbilitySystem/Abilityies/DodGameplayAbility.h"
#include "Animation/DodAnimInstance.h"


UDodAbilitySystemComponent::UDodAbilitySystemComponent()
{
}

void UDodAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			UDodGameplayAbility* AbilityCDO = Cast<UDodGameplayAbility>(AbilitySpec.Ability);
			if (!AbilityCDO)
			{
				continue;
			}

			if (AbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
			{
				TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
				for (UGameplayAbility* AbilityInstance : Instances)
				{
					UDodGameplayAbility* DodAbilityInstance = Cast<UDodGameplayAbility>(AbilityInstance);
					if (DodAbilityInstance)
					{
						DodAbilityInstance->OnPawnAvatarSet();
					}
				}
			}
			else
			{
				AbilityCDO->OnPawnAvatarSet();
			}
		}

		if (UDodGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UDodGlobalAbilitySystem>(GetWorld()))
		{
			GlobalAbilitySystem->RegisterASC(this);
		}

		if (UDodAnimInstance* DodAnimInst = Cast<UDodAnimInstance>(ActorInfo->GetAnimInstance()))
		{
			DodAnimInst->InitializeWithAbilitySystem(this);
		}

		TryActivateAbilitiesOnSpawn();
	}
}

void UDodAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (const UDodGameplayAbility* DodAbilityCDO = Cast<UDodGameplayAbility>(AbilitySpec.Ability))
		{
			DodAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
		}
	}
}
