#include "AbilitySystem/Abilityies/DodGameplayAbility_AutoSpawn.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "DodGameplayTags.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "AbilitySystem/Abilityies/DodGameplayAbility_Reset.h"
#include "Character/Comp/DodHealthComponent.h"
#include "GameFramework/AsyncAction_ListenForGameplayMessage.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/DodGameMode.h"
#include "GameMode/GameState/DodGameState.h"
#include "Interaction/DodInteractionDurationMessage.h"
#include "Kismet/GameplayStatics.h"
#include "Messages/DodVerbMessage.h"

UDodGameplayAbility_AutoSpawn::UDodGameplayAbility_AutoSpawn(const FObjectInitializer& ObjectInitializer)
{
	ActivationPolicy = EDodAbilityActivationPolicy::OnSpawn;
	ActivationGroup = EDodAbilityActivationGroup::Independent;

	AbilityTags.AddTag(DodGameplayTags::Ability_Behavior_SurvivesDeath);
}

void UDodGameplayAbility_AutoSpawn::OnAbilityAdded()
{
	Super::OnAbilityAdded();

	UUIExtensionSubsystem* UIExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
	if (UIExtensionSubsystem)
	{
		// UIExtensionSubsystem->RegisterExtensionAsWidgetForContext();
	}
}

void UDodGameplayAbility_AutoSpawn::OnAbilityRemoved()
{
	Super::OnAbilityRemoved();
	ClearDeathListener();
}

void UDodGameplayAbility_AutoSpawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ReadyForSpawn();
}

void UDodGameplayAbility_AutoSpawn::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                               bool bReplicateEndAbility, bool bWasCancelled)
{
	Extension.Unregister();
	ClearDeathListener();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDodGameplayAbility_AutoSpawn::OnPawnAvatarSet()
{
	Super::OnPawnAvatarSet();

	ReadyForSpawn();
}

void UDodGameplayAbility_AutoSpawn::ReadyForSpawn()
{
	if (!bIsListeningForReset)
	{
		UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
		MessageSystem.RegisterListener(DodGameplayTags::GameplayEvent_Reset, this, &ThisClass::OnReceiveReset);

		bIsListeningForReset = true;
	}
	if (IsAvatarDeadOrDying())
	{
		OnDeathStarted(GetAvatarActorFromActorInfo());
	}
	else
	{
		BindDeathListener();
	}
}

void UDodGameplayAbility_AutoSpawn::BindDeathListener()
{
	ClearDeathListener();

	LastBoundAvatarActor = GetAvatarActorFromActorInfo();
	if (LastBoundAvatarActor)
	{
		LastBoundAvatarActor->OnEndPlay.AddDynamic(this, &ThisClass::OnAvatarEndPlay);
		LastBoundHealthComponent = UDodHealthComponent::FindHealthComponent(LastBoundAvatarActor);
		if (LastBoundHealthComponent)
		{
			LastBoundHealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
		}
	}
}

void UDodGameplayAbility_AutoSpawn::ClearDeathListener()
{
	if (LastBoundAvatarActor)
	{
		LastBoundAvatarActor->OnEndPlay.RemoveDynamic(this, &ThisClass::OnAvatarEndPlay);
	}
	if (LastBoundHealthComponent)
	{
		LastBoundHealthComponent->OnDeathStarted.RemoveDynamic(this, &ThisClass::OnDeathStarted);
		LastBoundHealthComponent = nullptr;
	}
}

bool UDodGameplayAbility_AutoSpawn::IsAvatarDeadOrDying() const
{
	bool bHasDeadOrDyingAvatar = false;
	if (GetAvatarActorFromActorInfo())
	{
		UDodHealthComponent* DHC = UDodHealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo());
		if (DHC && DHC->IsDeadOrDying())
		{
			bHasDeadOrDyingAvatar = true;
		}
	}
	return bHasDeadOrDyingAvatar;
}

void UDodGameplayAbility_AutoSpawn::OnReceiveReset(FGameplayTag Channel, const FDodPlayerResetMessage& Payload)
{
	if (Payload.OwnerPlayerState == GetOwningActorFromActorInfo())
	{
		AvatarEndPlay();
	}
}

void UDodGameplayAbility_AutoSpawn::OnAvatarEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	AvatarEndPlay();
}

void UDodGameplayAbility_AutoSpawn::AvatarEndPlay()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		ControllerToReset = GetControllerFromActorInfo();
		ClearDeathListener();
		ContinueEndPlay();
	}
}

void UDodGameplayAbility_AutoSpawn::ContinueEndPlay()
{
	bShouldFinishRestart = false;
	UAbilitySystemComponent* ASC =
		UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ControllerToReset->PlayerState);
	if (ASC)
	{
		EndDeathAbilities(ASC);
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ThisClass::SpawnPlayer, .1f, false);
	}
}

void UDodGameplayAbility_AutoSpawn::SpawnPlayer()
{
	GWorld->GetTimerManager().ClearTimer(RespawnTimerHandle);
	ADodGameMode* DodGM = Cast<ADodGameMode>(GetWorld()->GetAuthGameMode());
	if (DodGM)
	{
		DodGM->RequestPlayerRestartNextFrame(ControllerToReset, true);
	}
	ADodGameState* DodGS = Cast<ADodGameState>(GetWorld()->GetGameState());
	if (DodGS)
	{
		FDodVerbMessage NetMessage;
		NetMessage.Verb = DodGameplayTags::Ability_Respawn_Completed_Message;
		NetMessage.Instigator = GetOwningPlayerState();
		DodGS->MulticastReliableMessageToClients(NetMessage);
		if (UKismetSystemLibrary::IsDedicatedServer(GetWorld()))
		{
			UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(
				DodGameplayTags::Ability_Respawn_Completed_Message, NetMessage);
		}
	}
}

AActor* UDodGameplayAbility_AutoSpawn::GetOwningPlayerState()
{
	if (UDodAbilitySystemComponent* DodASC = GetDodAbilitySystemComponentFromActorInfo())
	{
		return DodASC->GetOwner();
	}
	return nullptr;
}

void UDodGameplayAbility_AutoSpawn::EndDeathAbilities(UAbilitySystemComponent* ASC)
{
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;
	FGameplayTagContainer Tags;
	Tags.AddTag(DodGameplayTags::Ability_Type_StatusChange_Death);
	ASC->FindAllAbilitiesWithTags(AbilityHandles, Tags);
	for (FGameplayAbilitySpecHandle& Handle : AbilityHandles)
	{
		FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromHandle(Handle);
		UDodGameplayAbility* AbilityInstance = Cast<UDodGameplayAbility>(AbilitySpec->GetPrimaryInstance());
		if (!AbilityInstance)
		{
			AbilityInstance = Cast<UDodGameplayAbility>(AbilitySpec->Ability);
		}

		if (AbilityInstance)
		{
			AbilityInstance->EndCurrentAbility(true, false);
		}
	}
}

void UDodGameplayAbility_AutoSpawn::OnDeathStarted(AActor* OwningActor)
{
	ClearDeathListener();
	ControllerToReset = GetControllerFromActorInfo();
	if (!ControllerToReset)
	{
		return;
	}
	FDodInteractionDurationMessage NetMessage;
	NetMessage.Instigator = GetOwningPlayerState();
	NetMessage.Duration = RespawnDelayDuration;
	UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(DodGameplayTags::Ability_Respawn_Duration_Message,
	                                                            NetMessage);

	if (HasAuthority(&CurrentActivationInfo))
	{
		bShouldFinishRestart = true;
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (bShouldFinishRestart)
			{
				ContinueEndPlay();
			}
		}), RespawnDelayDuration, false);
	}
}
