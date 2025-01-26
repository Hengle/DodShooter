#include "AbilitySystem/DodAbilitySystemComponent.h"

#include "DodGlobalAbilitySystem.h"
#include "AbilitySystem/Abilityies/DodGameplayAbility.h"
#include "Animation/DodAnimInstance.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");

UDodAbilitySystemComponent::UDodAbilitySystemComponent()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UDodAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc,
                                                       bool bReplicateCancelAbility)
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.IsActive())
		{
			continue;
		}

		UDodGameplayAbility* DodAbilityCDO = Cast<UDodGameplayAbility>(AbilitySpec.Ability);
		if (!DodAbilityCDO)
		{
			continue;
		}

		if (DodAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			// Cancel all the spawned instances, not the CDO.
			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
			for (UGameplayAbility* AbilityInstance : Instances)
			{
				UDodGameplayAbility* DodAbilityInstance = CastChecked<UDodGameplayAbility>(AbilityInstance);

				if (ShouldCancelFunc(DodAbilityInstance, AbilitySpec.Handle))
				{
					if (DodAbilityInstance->CanBeCanceled())
					{
						DodAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(),
						                                   DodAbilityInstance->GetCurrentActivationInfo(),
						                                   bReplicateCancelAbility);
					}
				}
			}
		}
		else
		{
			// Cancel the non-instanced ability CDO.
			if (ShouldCancelFunc(DodAbilityCDO, AbilitySpec.Handle))
			{
				// Non-instanced abilities can always be canceled.
				check(DodAbilityCDO->CanBeCanceled());
				DodAbilityCDO->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(),
				                              FGameplayAbilityActivationInfo(), bReplicateCancelAbility);
			}
		}
	}
}

void UDodAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UDodAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UDodAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UDodGameplayAbility* AbilityCDO = Cast<UDodGameplayAbility>(AbilitySpec->Ability);
				if (AbilityCDO && AbilityCDO->GetActivationPolicy() == EDodAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UDodGameplayAbility* AbilityCDO = Cast<UDodGameplayAbility>(AbilitySpec->Ability);

					if (AbilityCDO &&
						AbilityCDO->GetActivationPolicy() == EDodAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UDodAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
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

bool UDodAbilitySystemComponent::IsActivationGroupBlocked(EDodAbilityActivationGroup Group) const
{
	bool bBlocked = false;

	switch (Group)
	{
	case EDodAbilityActivationGroup::Independent:
		bBlocked = false;
		break;

	case EDodAbilityActivationGroup::Exclusive_Replaceable:
	case EDodAbilityActivationGroup::Exclusive_Blocking:
		bBlocked = ActivationGroupCounts[(uint8)EDodAbilityActivationGroup::Exclusive_Blocking] > 0;
		break;

	default:
		break;
	}

	return bBlocked;
}

void UDodAbilitySystemComponent::GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle,
                                                      FGameplayAbilityActivationInfo ActivationInfo,
                                                      FGameplayAbilityTargetDataHandle& OutTargetDataHandle)
{
	TSharedPtr<FAbilityReplicatedDataCache> ReplicatedData = AbilityTargetDataMap.Find(
		FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, ActivationInfo.GetActivationPredictionKey()));
	if (ReplicatedData.IsValid())
	{
		OutTargetDataHandle = ReplicatedData->TargetData;
	}
}

void UDodAbilitySystemComponent::GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags,
                                                                        FGameplayTagContainer& OutActivationRequired,
                                                                        FGameplayTagContainer& OutActivationBlocked)
const
{
	/*if (TagRelationshipMapping)
	{
		TagRelationshipMapping->GetRequiredAndBlockedActivationTags(AbilityTags, &OutActivationRequired, &OutActivationBlocked);
	}*/
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
