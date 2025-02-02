#include "AbilitySystem/Abilityies/DodGameplayAbility_WeaponFire.h"

#include "AbilitySystemComponent.h"
#include "Character/DodCharacter.h"
#include "Character/DodShooter.h"
#include "GameplayCueFunctionLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon/DodProjectile.h"
#include "Weapon/DodRangedWeaponInstance.h"
#include "Weapon/WeaponBase.h"

UDodGameplayAbility_WeaponFire::UDodGameplayAbility_WeaponFire(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FGameplayTag InputTag = FGameplayTag::RequestGameplayTag(FName("InputTag.Weapon.Fire"));
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = InputTag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UDodGameplayAbility_WeaponFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo,
                                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsLocallyControlled())
	{
		OnTargetDataReadyCallback(FGameplayAbilityTargetDataHandle(), FGameplayTag());
	}
	if (ADodShooter* Player = Cast<ADodShooter>(GetDodCharacterFromActorInfo()))
	{
		Player->ArmMesh->GetAnimInstance()->Montage_Play(ArmFireMontage.LoadSynchronous());
	}
	StartFire();

	GetWorld()->GetTimerManager().SetTimer(EndTimer, FTimerDelegate::CreateLambda([&]
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}), FireDelayTimeSecs, false);
}

void UDodGameplayAbility_WeaponFire::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo))
	{
		if (ScopeLockCount > 0)
		{
			WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &ThisClass::EndAbility, Handle, ActorInfo,
			                                                      ActivationInfo, bReplicateEndAbility, bWasCancelled));
			return;
		}

		UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
		check(MyAbilityComponent);

		MyAbilityComponent->ConsumeClientReplicatedTargetData(CurrentSpecHandle,
		                                                      CurrentActivationInfo.GetActivationPredictionKey());

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

void UDodGameplayAbility_WeaponFire::OnAbilityAdded()
{
	Super::OnAbilityAdded();
}

void UDodGameplayAbility_WeaponFire::StartFire()
{
	FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(GE_Damage, GetAbilityLevel());
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
	{
		ADodCharacter* Character = GetDodCharacterFromActorInfo();
		UDodRangedWeaponInstance* Weapon = GetWeaponInstance();
		if (Weapon)
		{
			TSubclassOf<ADodProjectile> ProjectileClass = Weapon->ProjectileClass.LoadSynchronous();
			if (Character)
			{
				AWeaponBase* WeaponActor = Cast<AWeaponBase>(Weapon->GetSpawnedActor());
				FTransform FireTransform = FTransform::Identity;
				FireTransform.SetLocation(WeaponActor->VM_Receiver->GetComponentLocation());
				FireTransform.SetRotation(Character->GetControlRotation().Quaternion());

				FActorSpawnParameters SpawnParameters;
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// TODO: 客户端预测生成子弹
				ADodProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADodProjectile>(
					ProjectileClass,
					FireTransform,
					GetOwningActorFromActorInfo(),
					Character,
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
				Projectile->FinishSpawning(FireTransform);
				
				float InitialSpeed = 60000.f;
				Projectile->InitializeProjectile(InitialSpeed);
			}
		}
	}
}
