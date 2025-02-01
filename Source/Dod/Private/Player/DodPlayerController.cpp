#include "Player/DodPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "Equipment/DodQuickBarComponent.h"
#include "Inventory/DodInventoryManagerComponent.h"
#include "Player/DodPlayerState.h"
#include "Team/DodTeamAgentInterface.h"
#include "Weapon/DodWeaponStateComponent.h"

ADodPlayerController::ADodPlayerController(const FObjectInitializer& FObjectInitializer)
	: Super(FObjectInitializer)
{
	InventoryComponent = CreateDefaultSubobject<UDodInventoryManagerComponent>(TEXT("InventoryComponent"));
	QuickBarComponent = CreateDefaultSubobject<UDodQuickBarComponent>(TEXT("QuickBarComponent"));
	WeaponStateComponent = CreateDefaultSubobject<UDodWeaponStateComponent>(TEXT("WeaponStateComponent"));
}

ADodPlayerState* ADodPlayerController::GetDodPlayerState() const
{
	return Cast<ADodPlayerState>(PlayerState);
}

UDodAbilitySystemComponent* ADodPlayerController::GetDodAbilitySystemComponent() const
{
	const ADodPlayerState* PS = GetDodPlayerState();
	return PS ? PS->GetDodAbilitySystemComponent() : nullptr;
}

void ADodPlayerController::OnUnPossess()
{
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	UnEquip();

	Super::OnUnPossess();
}

void ADodPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void ADodPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void ADodPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();
}

void ADodPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UDodAbilitySystemComponent* ASC = GetDodAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ADodPlayerController::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	// 不允许通过PlayerController设置TeamId，通过PlayerState设置
}

FGenericTeamId ADodPlayerController::GetGenericTeamId() const
{
	if (const IDodTeamAgentInterface* PSWithTeamInterface = Cast<IDodTeamAgentInterface>(PlayerState))
	{
		return PSWithTeamInterface->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

FOnDodTeamIndexChangedDelegate* ADodPlayerController::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void ADodPlayerController::UnEquip()
{
	if (QuickBarComponent)
	{
		const TArray<UDodInventoryItemInstance*>& Slots = QuickBarComponent->GetSlots();
		for (int32 Idx = 0; Idx < Slots.Num(); ++Idx)
		{
			QuickBarComponent->RemoveItemFromSlot(Idx);
		}
	}
	if (InventoryComponent)
	{
		const TArray<UDodInventoryItemInstance*>& Items = InventoryComponent->GetAllItems();
		for (UDodInventoryItemInstance* Item : Items)
		{
			InventoryComponent->RemoveItemInstance(Item);
		}
	}
}

void ADodPlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}

void ADodPlayerController::BroadcastOnPlayerStateChanged()
{
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (LastSeenPlayerState != nullptr)
	{
		if (IDodTeamAgentInterface* PlayerStateTeamInterface = Cast<IDodTeamAgentInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().RemoveAll(this);
		}
	}

	// Bind to the new player state, if any
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (PlayerState != nullptr)
	{
		if (IDodTeamAgentInterface* PlayerStateTeamInterface = Cast<IDodTeamAgentInterface>(PlayerState))
		{
			NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().AddDynamic(
				this, &ThisClass::OnPlayerStateChangedTeam);
		}
	}

	// Broadcast the team change (if it really has)
	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);

	LastSeenPlayerState = PlayerState;
}
