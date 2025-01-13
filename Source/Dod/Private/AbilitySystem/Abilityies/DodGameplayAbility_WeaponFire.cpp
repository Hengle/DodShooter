#include "AbilitySystem/Abilityies/DodGameplayAbility_WeaponFire.h"

#include "AbilitySystemComponent.h"
#include "Character/DodCharacter.h"
#include "Character/DodShooter.h"
#include "GameplayCueFunctionLibrary.h"

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
		StartRangedWeaponTargeting();
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

	/*UAsyncAction_ListenForGameplayMessage* Message = UAsyncAction_ListenForGameplayMessage::ListenForGameplayMessages(
		GetWorld(),
		FGameplayTag::RequestGameplayTag(FName("Ability.PlayMontageOnActivateFail.Message")),
		FDodAbilityMontageFailureMessage::StaticStruct(),
		EGameplayMessageMatch::ExactMatch);
	Message->OnMessageReceived.AddDynamic(this, &ThisClass::ReceiveFailMessage);*/
}

void UDodGameplayAbility_WeaponFire::RangedWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData)
{
	Super::RangedWeaponTargetDataReady(TargetData);

	K2_ExecuteGameplayCueWithParams(GameplayCueTagFiring, GCNParameter);

	for (TSharedPtr<FGameplayAbilityTargetData> Data : TargetData.Data)
	{
		FHitResult CurrentHitResult = *Data->GetHitResult();
		GCNParameter.Location = (CurrentHitResult.bBlockingHit
			                         ? CurrentHitResult.ImpactPoint
			                         : CurrentHitResult.TraceEnd);
		GCNParameter.Normal = CurrentHitResult.ImpactNormal;
		GCNParameter.PhysicalMaterial = CurrentHitResult.PhysMaterial;
		if (CurrentHitResult.bBlockingHit)
		{
			K2_ExecuteGameplayCueWithParams(GameplayCueTagFiring,
			                                UGameplayCueFunctionLibrary::MakeGameplayCueParametersFromHitResult(
				                                CurrentHitResult));
		}
		if (HasAuthority(&CurrentActivationInfo) && IsValid(FieldActorToSpawnOnImpact))
		{
			FVector SpawnLocation = FVector(CurrentHitResult.ImpactPoint);
			GetWorld()->SpawnActor(FieldActorToSpawnOnImpact, &SpawnLocation);
		}
	}
	if (HasAuthority(&CurrentActivationInfo))
	{
		/*TArray<FActiveGameplayEffectHandle> Handles =
			ApplyGameplayEffectToTarget(CurrentSpecHandle,
			                            CurrentActorInfo,
			                            CurrentActivationInfo,
			                            TargetData,
			                            GE_Damage,
			                            1);*/
	}
}

void UDodGameplayAbility_WeaponFire::StartFire()
{
	/*UDodRangedWeaponInstance* Weapon = GetWeaponInstance();
	if (IsLocallyControlled())
	{
	}*/
}

void UDodGameplayAbility_WeaponFire::SpawnProjectile(FVector FireLocation)
{
}

void UDodGameplayAbility_WeaponFire::Server_SpawnProjectile_Implementation(FVector FireLocation)
{
	if (!IsLocallyControlled())
	{
		SpawnProjectile(FireLocation);
	}
}
