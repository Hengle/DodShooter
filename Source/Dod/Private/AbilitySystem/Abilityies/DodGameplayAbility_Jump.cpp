#include "AbilitySystem/Abilityies/DodGameplayAbility_Jump.h"

#include "Character/DodCharacter.h"

UDodGameplayAbility_Jump::UDodGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UDodGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayTagContainer* SourceTags,
                                                  const FGameplayTagContainer* TargetTags,
                                                  FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ADodCharacter* DodCharacter = Cast<ADodCharacter>(ActorInfo->AvatarActor.Get());
	if (!DodCharacter || !DodCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UDodGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDodGameplayAbility_Jump::CharacterJumpStart()
{
	if (ADodCharacter* DodCharacter = GetDodCharacterFromActorInfo())
	{
		if (DodCharacter->IsLocallyControlled() && !DodCharacter->bPressedJump)
		{
			DodCharacter->UnCrouch();
			DodCharacter->Jump();
		}
	}
}

void UDodGameplayAbility_Jump::CharacterJumpStop()
{
	if (ADodCharacter* DodCharacter = GetDodCharacterFromActorInfo())
	{
		if (DodCharacter->IsLocallyControlled() && DodCharacter->bPressedJump)
		{
			DodCharacter->StopJumping();
		}
	}
}
