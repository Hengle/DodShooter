#include "AbilitySystem/Abilityies/DodGameplayAbility_Inspect.h"

#include "Character/DodShooter.h"
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"

void UDodGameplayAbility_Inspect::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	float AnimTime = 0.1f;
	if (ADodShooter* DodShooter = Cast<ADodShooter>(GetDodCharacterFromActorInfo()))
	{
		if (UAnimInstance* ArmAnim = DodShooter->ArmMesh->GetAnimInstance())
		{
			AnimTime = ArmAnim->Montage_Play(VM_InspectMontage);
		}
		if (UAnimInstance* BodyAnim = DodShooter->GetMesh()->GetAnimInstance())
		{
			BodyAnim->Montage_Play(VM_InspectMontage);
		}
		DodShooter->OnCharacterMovementUpdated.AddDynamic(this, &ThisClass::CheckForMovement);
	}
	GetWorld()->GetTimerManager().SetTimer(AnimTimer, FTimerDelegate::CreateLambda([&]()
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}), AnimTime, false);
}

void UDodGameplayAbility_Inspect::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo,
                                             bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (ADodShooter* DodShooter = Cast<ADodShooter>(GetDodCharacterFromActorInfo()))
	{
		DodShooter->OnCharacterMovementUpdated.RemoveAll(this);
	}
}

void UDodGameplayAbility_Inspect::CheckForMovement(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	if (OldVelocity.Size() > 0.f)
	{
		if (ADodShooter* DodShooter = Cast<ADodShooter>(GetDodCharacterFromActorInfo()))
		{
			DodShooter->OnCharacterMovementUpdated.RemoveAll(this);
			if (UAnimInstance* ArmAnim = DodShooter->ArmMesh->GetAnimInstance())
			{
				ArmAnim->Montage_Stop(.1f);
			}
			if (UAnimInstance* BodyAnim = DodShooter->GetMesh()->GetAnimInstance())
			{
				BodyAnim->Montage_Stop(.1f);
			}
		}
		GetWorld()->GetTimerManager().ClearTimer(AnimTimer);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
