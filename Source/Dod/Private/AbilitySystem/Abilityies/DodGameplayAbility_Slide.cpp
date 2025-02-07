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
		GetDirection(ActorFacing, ActorLastMovementInput, ActorMovement);
		// TODO：移动模式切换到全向移动

		if (SelectDirectionalMontage(ActorFacing, ActorLastMovementInput, BodyDirMontage) &&
			!ActorMovement.IsNearlyZero())
		{
			if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, ActivationInfo))
			{
				if (ADodCharacter* Character = GetDodCharacterFromActorInfo())
				{
					if (!Character->bIsCrouched)
					{
						Character->Crouch();
					}
				}
				if (!HasAuthority(&ActivationInfo))
				{
					Server_SendInfo(ActorMovement, BodyDirMontage);
				}
				PlayMontage();
			}
		}

		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UDodGameplayAbility_Slide::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!bWasCancelled)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (ASC && ASC->AbilityActorInfo->MovementComponent.IsValid())
		{
			if (UCharacterMovementComponent* MovementComponent =
				Cast<UCharacterMovementComponent>(ASC->AbilityActorInfo->MovementComponent.Get()))
			{
				MovementComponent->RemoveRootMotionSourceByID(SlideRootMotionSourceID);
			}
		}

		if (ADodCharacter* Character = GetDodCharacterFromActorInfo())
		{
			if (UAnimInstance* BodyAnim = Character->GetMesh()->GetAnimInstance())
			{
				if (BodyAnim->Montage_IsPlaying(BodyDirMontage))
				{
					BodyAnim->Montage_Stop(.2f, BodyDirMontage);
				}
			}
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

bool UDodGameplayAbility_Slide::SelectDirectionalMontage(const FVector& FacingDirection,
                                                         const FVector& MovementDirection,
                                                         UAnimMontage*& BodyAnimMontage)
{
	FRotator FacingRotation = FacingDirection.ToOrientationRotator();
	FRotator MovementRotation = MovementDirection.ToOrientationRotator();
	float Angle = (FacingRotation - MovementRotation).Yaw;
	if (FMath::Abs(Angle) > 90.f)
	{
		return false;
	}
	if (FMath::Abs(Angle) < 15.f)
	{
		BodyAnimMontage = SlideMontage8;
	}
	else if (Angle < 0)
	{
		BodyAnimMontage = SlideMontage7;
	}
	else
	{
		BodyAnimMontage = SlideMontage9;
	}
	return true;
}

void UDodGameplayAbility_Slide::PlayMontage()
{
	if (ADodCharacter* Character = GetDodCharacterFromActorInfo())
	{
		UAnimInstance* BodyAnim = Character->GetMesh()->GetAnimInstance();
		BodyAnim->Montage_Play(BodyDirMontage);
	}
	ApplyRootMotion();
}

void UDodGameplayAbility_Slide::ApplyRootMotion()
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
			SlideRootMotionSourceID = MovementComponent->ApplyRootMotionSource(ConstantForce);
		}
	}
}

void UDodGameplayAbility_Slide::Server_SendInfo_Implementation(FVector Direction, UAnimMontage* BodyMontage)
{
	ActorMovement = Direction;
	BodyDirMontage = BodyMontage;
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);

	PlayMontage();
}
