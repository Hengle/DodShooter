#include "AbilitySystem/Abilityies/DodGameplayAbility_RangedWeapon.h"

#include "AbilitySystemComponent.h"
#include "Equipment/DodEquipmentManagerComponent.h"
#include "Weapon/DodRangedWeaponInstance.h"

UDodRangedWeaponInstance* UDodGameplayAbility_RangedWeapon::GetWeaponInstance() const
{
	return Cast<UDodRangedWeaponInstance>(GetAssociatedEquipment());
}

bool UDodGameplayAbility_RangedWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                          const FGameplayAbilityActorInfo* ActorInfo,
                                                          const FGameplayTagContainer* SourceTags,
                                                          const FGameplayTagContainer* TargetTags,
                                                          FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (bResult)
	{
		if (GetWeaponInstance() == nullptr)
		{
			bResult = false;
		}
	}

	return bResult;
}

void UDodGameplayAbility_RangedWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                       const FGameplayAbilityActorInfo* ActorInfo,
                                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                                       const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	OnTargetDataReadyCallbackDelegateHandle = MyAbilityComponent->AbilityTargetDataSetDelegate(
		CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).AddUObject(
		this, &ThisClass::OnTargetDataReadyCallback);

	UDodRangedWeaponInstance* WeaponData = GetWeaponInstance();
	check(WeaponData);
	WeaponData->UpdateFiringTime();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDodGameplayAbility_RangedWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle,
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

		MyAbilityComponent->AbilityTargetDataSetDelegate(CurrentSpecHandle,
		                                                 CurrentActivationInfo.GetActivationPredictionKey()).Remove(
			OnTargetDataReadyCallbackDelegateHandle);
		MyAbilityComponent->ConsumeClientReplicatedTargetData(CurrentSpecHandle,
		                                                      CurrentActivationInfo.GetActivationPredictionKey());

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

int32 UDodGameplayAbility_RangedWeapon::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
	for (int32 Idx = 0; Idx < HitResults.Num(); ++Idx)
	{
		const FHitResult& CurHitResult = HitResults[Idx];
		if (CurHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()))
		{
			// If we hit a pawn, we're good
			return Idx;
		}
		AActor* HitActor = CurHitResult.HitObjectHandle.FetchActor();
		if ((HitActor != nullptr) && (HitActor->GetAttachParentActor() != nullptr) && (Cast<APawn>(
			HitActor->GetAttachParentActor()) != nullptr))
		{
			// If we hit something attached to a pawn, we're good
			return Idx;
		}
	}

	return INDEX_NONE;
}

FVector UDodGameplayAbility_RangedWeapon::GetWeaponTargetingSourceLocation() const
{
	// Use Pawn's location as a base
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);

	const FVector SourceLoc = AvatarPawn->GetActorLocation();
	const FQuat SourceRot = AvatarPawn->GetActorQuat();

	FVector TargetingSourceLocation = SourceLoc;

	return TargetingSourceLocation;
}

void UDodGameplayAbility_RangedWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData,
                                                                 FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	FGameplayAbilityTargetDataHandle LocalTargetDataHandle(
		MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));
	const bool bShouldNotifyServer = CurrentActorInfo->IsLocallyControlled() && !CurrentActorInfo->IsNetAuthority();
	if (bShouldNotifyServer)
	{
		MyAbilityComponent->CallServerSetReplicatedTargetData(CurrentSpecHandle,
		                                                      CurrentActivationInfo.GetActivationPredictionKey(),
		                                                      LocalTargetDataHandle, ApplicationTag,
		                                                      MyAbilityComponent->ScopedPredictionKey);
	}

	// See if we still have ammo
	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		UDodRangedWeaponInstance* WeaponData = GetWeaponInstance();
		check(WeaponData);
		// 散布
		// WeaponData->AddSpread();

		OnWeaponFireCompleted();
	}
	else
	{
		K2_EndAbility();
	}
}

void UDodGameplayAbility_RangedWeapon::OnWeaponFireCompleted_Implementation()
{
}
