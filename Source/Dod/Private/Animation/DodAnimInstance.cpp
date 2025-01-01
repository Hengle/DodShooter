#include "Animation/DodAnimInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "DodGameplayTags.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDodAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UDodAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!TryGetPawnOwner())
	{
		return;
	}

	UpdateLocationData(DeltaSeconds);
	UpdateRotationData(DeltaSeconds);
	UpdateVelocityData(DeltaSeconds);
	UpdateAccelerationData(DeltaSeconds);
	UpdateWallDetectionHeuristic();
	UpdateCharacterStateData(DeltaSeconds);
	UpdateBlendWeightData(DeltaSeconds);
	UpdateRootYawOffset(DeltaSeconds);
	UpdateAimingData();
	UpdateJumpFallData();

	bIsFirstUpdate = false;
}

void UDodAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(DodGameplayTags::Event_Movement_ADS,
		                              EGameplayTagEventType::NewOrRemoved).
		     AddUObject(this, &ThisClass::OnGameplayTagAdsChanged);
		ASC->RegisterGameplayTagEvent(DodGameplayTags::Event_Movement_Melee,
		                              EGameplayTagEventType::NewOrRemoved).
		     AddUObject(this, &ThisClass::OnGameplayTagMeleeChanged);
		ASC->RegisterGameplayTagEvent(DodGameplayTags::Event_Movement_Reload,
		                              EGameplayTagEventType::NewOrRemoved).
		     AddUObject(this, &ThisClass::OnGameplayTagReloadingChanged);
		ASC->RegisterGameplayTagEvent(DodGameplayTags::Event_Movement_WeaponFire,
		                              EGameplayTagEventType::NewOrRemoved).
		     AddUObject(this, &ThisClass::OnGameplayTagFiringChanged);
	}
}

void UDodAnimInstance::UpdateLocationData(float DeltaSeconds)
{
	const FVector ActorLocation = GetOwningActor()->GetActorLocation();

	DisplacementSinceLastUpdate = (ActorLocation - WorldLocation).Size2D();
	WorldLocation = ActorLocation;
	DisplacementSpeed = DeltaSeconds != 0.f ? DisplacementSinceLastUpdate / DeltaSeconds : 0.f;

	if (bIsFirstUpdate)
	{
		DisplacementSinceLastUpdate = 0.f;
		DisplacementSpeed = 0.f;
	}
}

void UDodAnimInstance::UpdateRotationData(float DeltaSeconds)
{
	FRotator ActorRotation = GetOwningActor()->GetActorRotation();

	YawDeltaSinceLastUpdate = ActorRotation.Yaw - WorldRotation.Yaw;
	YawDeltaSpeed = DeltaSeconds != 0.f ? YawDeltaSinceLastUpdate / DeltaSeconds : 0.f;

	WorldRotation = ActorRotation;

	if (bIsFirstUpdate)
	{
		YawDeltaSinceLastUpdate = 0.f;
	}
}

void UDodAnimInstance::UpdateVelocityData(float DeltaSeconds)
{
	bWasMovingLastUpdate = !LocalVelocity2D.IsNearlyZero();
	WorldVelocity = TryGetPawnOwner()->GetVelocity();
	WorldVelocity2D = FVector(WorldVelocity.X, WorldVelocity.Y, 0.f);
	LocalVelocity2D = WorldRotation.UnrotateVector(WorldVelocity2D);
	FVector TargetDirection = LocalVelocity2D.GetSafeNormal();
	LocalDirection2D = FMath::VInterpTo(LocalDirection2D, TargetDirection, DeltaSeconds, 15.f);
	LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(WorldVelocity2D, WorldRotation);
	LocalVelocityDirectionAngleWithOffset = LocalVelocityDirectionAngle - RootYawOffset;
	bHasVelocity = !FMath::IsNearlyEqual(LocalVelocity2D.Size2D(), 0);
}

void UDodAnimInstance::UpdateAccelerationData(float DeltaSeconds)
{
	if (ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner()))
	{
		WorldAcceleration2D = Character->GetCharacterMovement()->GetCurrentAcceleration();
		WorldAcceleration2D.Z = 0.f;
		LocalAcceleration2D = WorldRotation.UnrotateVector(WorldAcceleration2D);
	}
	bHasAcceleration = !FMath::IsNearlyEqual(WorldAcceleration2D.Size2D(), 0.f);
	PivotDirection2D = FMath::VInterpTo(PivotDirection2D,
	                                    WorldAcceleration2D.GetSafeNormal(),
	                                    DeltaSeconds,
	                                    10.f);
	PivotDirection2D.Normalize();
}

void UDodAnimInstance::UpdateWallDetectionHeuristic()
{
	float Dir = LocalAcceleration2D.GetSafeNormal().Dot(LocalVelocity2D.GetSafeNormal());
	bIsRunningIntoWall = LocalAcceleration2D.Size2D() > .1f &&
		LocalVelocity2D.Size2D() < 200.f &&
		Dir <= .6f && Dir >= -.6f;
}

void UDodAnimInstance::UpdateCharacterStateData(float DeltaSeconds)
{
	bWasCrouchingLastUpdate = bIsCrouching;
	bIsJumping = false;
	bIsFalling = false;
	if (ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner()))
	{
		if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
		{
			bIsOnGround = MovementComponent->IsMovingOnGround();
			bIsCrouching = MovementComponent->IsCrouching();
			bCrouchStateChanged = bIsCrouching != bWasCrouchingLastUpdate;

			if (MovementComponent->MovementMode == MOVE_Falling)
			{
				if (WorldVelocity.Z > 0.f)
				{
					bIsJumping = true;
				}
				else
				{
					bIsFalling = true;
				}
			}
		}
	}
	bAdsStateChanged = bIsAds != bWasAdsLastUpdate;
	bWasAdsLastUpdate = bIsAds;
	if (bIsFiring)
	{
		TimeSinceFiredWeapon = 0.f;
	}
	else
	{
		TimeSinceFiredWeapon += DeltaSeconds;
	}
}

void UDodAnimInstance::UpdateBlendWeightData(float DeltaSeconds)
{
	if (IsAnyMontagePlaying())
	{
		UpperBodyDynamicAdditiveWeight = 1.f;
	}
	else
	{
		UpperBodyDynamicAdditiveWeight = FMath::FInterpTo(UpperBodyDynamicAdditiveWeight,
		                                                  0.f,
		                                                  DeltaSeconds,
		                                                  6.f);
	}
}

void UDodAnimInstance::UpdateRootYawOffset(float DeltaSeconds)
{
	if (RootYawOffsetMode == ERootYawOffsetMode::Accumulate)
	{
		SetRootYawOffset(RootYawOffset - YawDeltaSinceLastUpdate);
	}
	else if (RootYawOffsetMode == ERootYawOffsetMode::BlendOut)
	{
		FFloatSpringState SpringState;
		float WantToRootYawOffset = UKismetMathLibrary::FloatSpringInterp(RootYawOffset,
		                                                                  0.f, SpringState, 80.f,
		                                                                  1.f, DeltaSeconds,
		                                                                  1.f, .5f);
		SetRootYawOffset(WantToRootYawOffset);
	}
	RootYawOffsetMode = ERootYawOffsetMode::Accumulate;
}

void UDodAnimInstance::UpdateAimingData()
{
	AimPitch = TryGetPawnOwner()->GetBaseAimRotation().Pitch;
}

void UDodAnimInstance::UpdateJumpFallData()
{
	if (bIsJumping)
	{
		TimeToJumpApex = WorldVelocity.Z / TryGetPawnOwner()->GetMovementComponent()->GetGravityZ();
	}
	else
	{
		TimeToJumpApex = 0.f;
	}
}

void UDodAnimInstance::SetRootYawOffset(float InRootYawOffset)
{
	if (bEnableRootYawOffset)
	{
		float ClampedYawOffset = FRotator::NormalizeAxis(InRootYawOffset);
		if (bIsCrouching)
		{
			RootYawOffset =
				FMath::ClampAngle(ClampedYawOffset,
				                  RootYawOffsetAngleClampCrouched.X,
				                  RootYawOffsetAngleClampCrouched.Y);
		}
		else
		{
			RootYawOffset =
				FMath::ClampAngle(ClampedYawOffset, RootYawOffsetAngleClamp.X, RootYawOffsetAngleClamp.Y);
		}
		AimYaw = RootYawOffset * -1.f;
	}
	else
	{
		RootYawOffset = 0.f;
		AimYaw = 0.f;
	}
}

void UDodAnimInstance::OnGameplayTagAdsChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (Tag == DodGameplayTags::Event_Movement_ADS)
	{
		bIsAds = NewCount > 0;
	}
}

void UDodAnimInstance::OnGameplayTagFiringChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (Tag == DodGameplayTags::Event_Movement_WeaponFire)
	{
		bIsFiring = NewCount > 0;
	}
}

void UDodAnimInstance::OnGameplayTagReloadingChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (Tag == DodGameplayTags::Event_Movement_Reload)
	{
		bIsReloading = NewCount > 0;
	}
}

void UDodAnimInstance::OnGameplayTagMeleeChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (Tag == DodGameplayTags::Event_Movement_Melee)
	{
		bIsMelee = NewCount > 0;
	}
}
