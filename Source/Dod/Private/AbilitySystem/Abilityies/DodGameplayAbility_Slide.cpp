#include "AbilitySystem/Abilityies/DodGameplayAbility_Slide.h"

#include "AbilitySystemComponent.h"
#include "DodGameplayTags.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Camera/DodCameraComponent.h"
#include "Character/DodCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UDodGameplayAbility_Slide::UDodGameplayAbility_Slide()
{
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = DodGameplayTags::GameplayEvent_Slide;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UDodGameplayAbility_Slide::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsLocallyControlled())
	{
		FVector ActorFacing, ActorLastMovementInput, ActorMovement;
		GetDirection(ActorFacing, ActorLastMovementInput, ActorMovement);
		if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, ActivationInfo))
		{
			FVector WorldDirection = GetDodCharacterFromActorInfo()->GetLastMovementInputVector();
			UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
			if (ASC && ASC->AbilityActorInfo->MovementComponent.IsValid())
			{
				if (UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(
					ASC->AbilityActorInfo->MovementComponent.Get()))
				{
					TSharedPtr<FRootMotionSource_ConstantForce> ConstantForce =
						MakeShared<FRootMotionSource_ConstantForce>();
					ConstantForce->AccumulateMode = ERootMotionAccumulateMode::Additive;
					ConstantForce->Priority = 5;
					ConstantForce->Force = WorldDirection * 800.f;
					ConstantForce->Duration = 1;
					ConstantForce->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::ClampVelocity;
					ConstantForce->FinishVelocityParams.SetVelocity = FVector(0, 0, 0);
					ConstantForce->FinishVelocityParams.ClampVelocity = 1000.f;
					uint16 RootMotionSourceID = MovementComponent->ApplyRootMotionSource(ConstantForce);
				}
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UDodGameplayAbility_Slide::GetDirection(FVector& Facing, FVector& LastMovementInput, FVector& Movement)
{
	if (ADodCharacter* Character = GetDodCharacterFromActorInfo())
	{
		Facing = Character->GetActorForwardVector();

		FVector ActorLocation = Character->GetActorLocation();
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		FNavLocation NavLocation;
		NavSys->ProjectPointToNavigation(ActorLocation, NavLocation, FVector(500.f, 500.f, 500.f));
		FVector FaceDirection = NavLocation.Location - ActorLocation;
		FaceDirection.Z = 0;
		FaceDirection.Normalize();

		if (Character->IsPlayerControlled())
		{
			LastMovementInput = Character->GetLastMovementInputVector();
			Movement = Character->CameraComponent->GetForwardVector();
		}
		else
		{
			LastMovementInput = FaceDirection;
			Movement = FaceDirection;
		}
	}
}
