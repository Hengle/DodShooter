#include "Character/Comp/DodPawnExtensionComponent.h"

#include "DodGameplayTags.h"
#include "AbilitySystem/DodAbilitySet.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"

const FName UDodPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UDodPawnExtensionComponent::UDodPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UDodPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                                    FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() && DesiredState == DodGameplayTags::InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == DodGameplayTags::InitState_Spawned && DesiredState == DodGameplayTags::InitState_DataAvailable)
	{
		const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

		if (bHasAuthority || bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}
	if (CurrentState == DodGameplayTags::InitState_DataAvailable &&
		DesiredState == DodGameplayTags::InitState_DataInitialized)
	{
		return Manager->HaveAllFeaturesReachedInitState(Pawn, DodGameplayTags::InitState_DataAvailable);
	}
	if (CurrentState == DodGameplayTags::InitState_DataInitialized &&
		DesiredState == DodGameplayTags::InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UDodPawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager,
                                                       FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	IGameFrameworkInitStateInterface::HandleChangeInitState(Manager, CurrentState, DesiredState);
}

void UDodPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		if (Params.FeatureState == DodGameplayTags::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void UDodPawnExtensionComponent::CheckDefaultInitialization()
{
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = {
		DodGameplayTags::InitState_Spawned,
		DodGameplayTags::InitState_DataAvailable,
		DodGameplayTags::InitState_DataInitialized,
		DodGameplayTags::InitState_GameplayReady
	};

	ContinueInitStateChain(StateChain);
}

void UDodPawnExtensionComponent::HandleControllerChanged()
{
	CheckDefaultInitialization();
}

void UDodPawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckDefaultInitialization();
}

void UDodPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void UDodPawnExtensionComponent::InitializeAbilitySystem(UDodAbilitySystemComponent* InAsc, AActor* InOwner)
{
	check(InAsc);
	check(InOwner);

	if (AbilitySystemComponent == InAsc)
	{
		return;
	}

	if (AbilitySystemComponent)
	{
		UninitializeAbilitySystem();
	}

	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAvatar = InAsc->GetAvatarActor();

	if (ExistingAvatar && ExistingAvatar != Pawn)
	{
		ensure(!ExistingAvatar->HasAuthority());

		if (UDodPawnExtensionComponent* OtherExtensionComponent = FindPawnExtensionComponent(ExistingAvatar))
		{
			OtherExtensionComponent->UninitializeAbilitySystem();
		}
	}

	AbilitySystemComponent = InAsc;
	AbilitySystemComponent->InitAbilityActorInfo(InOwner, Pawn);

	for (const UDodAbilitySet* AbilitySet : PawnData.AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}

void UDodPawnExtensionComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent = nullptr;
}

void UDodPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	ensure(TryToChangeInitState(DodGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}
