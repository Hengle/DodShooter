#include "AbilitySystem/Abilityies/DodGameplayAbility_WeaponFire.h"

#include "AbilitySystemComponent.h"
#include "Weapon/DodRangedWeaponInstance.h"

void UDodGameplayAbility_WeaponFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo,
                                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                                     const FGameplayEventData* TriggerEventData)
{
	// 以防万一，先从摄像机做一次射线检测进行追踪

	UAbilitySystemComponent* AbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();

	check(AbilityComponent);

	UDodRangedWeaponInstance* WeaponData = GetWeaponInstance();
	check(WeaponData);
	WeaponData->UpdateFiringTime();

	FVector FireLocation;
	SpawnProjectile(FireLocation);
	Server_SpawnProjectile(FireLocation);

	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		// 执行后坐力
		// UDodRangedWeaponInstance* WeaponData = GetWeaponInstance();
		// check(WeaponData);
		// WeaponData->AddSpread();

		// OnRangedWeaponTargetDataReady(LocalTargetDataHandle);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
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

void UDodGameplayAbility_WeaponFire::SpawnProjectile(FVector FireLocation)
{
}

void UDodGameplayAbility_WeaponFire::Server_SpawnProjectile_Implementation(FVector FireLocation)
{
	SpawnProjectile(FireLocation);
}
