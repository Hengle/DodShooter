#include "Character/DodShooter.h"

#include "DodGameplayTags.h"
#include "Camera/DodCameraComponent.h"
#include "Character/Comp/DodPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Equipment/DodEquipmentManagerComponent.h"
#include "Equipment/DodQuickBarComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/DodExperienceManagerComponent.h"
#include "Inventory/DodInventoryItemDefinition.h"
#include "Inventory/DodInventoryManagerComponent.h"

ADodShooter::ADodShooter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(SpringArmComponent);

	CameraComponent->SetupAttachment(ArmMesh, TEXT("tag_camera"));
}

void ADodShooter::ChangeToFirstPerson()
{
	HeadMesh->SetVisibility(false);
	GetMesh()->SetVisibility(false);
	ArmMesh->SetVisibility(true);
}

void ADodShooter::ChangeToThirdPerson()
{
	HeadMesh->SetVisibility(true);
	GetMesh()->SetVisibility(true);
	ArmMesh->SetVisibility(false);
}

void ADodShooter::ChooseViewPerson()
{
	if (IsPlayerControlled() && IsLocallyControlled())
	{
		ChangeToFirstPerson();
	}
	else
	{
		ChangeToThirdPerson();
	}
}

void ADodShooter::BeginPlay()
{
	Super::BeginPlay();

	HeadMesh->SetCastShadow(true);
	HeadMesh->SetCastHiddenShadow(true);
	GetMesh()->SetCastShadow(true);
	GetMesh()->SetCastHiddenShadow(true);
	ArmMesh->SetCastShadow(false);

	SetActorHiddenInGame(true);

	ChooseViewPerson();

	FActorInitStateChangedBPDelegate Delegate;
	Delegate.BindUFunction(this, FName("ShowPawnAgainNextFrame"));

	PawnExtComponent->RegisterAndCallForInitStateChange(DodGameplayTags::InitState_GameplayReady,
	                                                    Delegate,
	                                                    true
	);
}

void ADodShooter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ChooseViewPerson();
	if (HasAuthority())
	{
		UDodExperienceManagerComponent* ExperienceComponent =
			GetWorld()->GetGameState()->FindComponentByClass<UDodExperienceManagerComponent>();
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(
			FOnDodExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::HandleExperienceLoaded));
	}
}

void ADodShooter::UnPossessed()
{
	Super::UnPossessed();
	ChooseViewPerson();
}

void ADodShooter::DestroyDueToDeath()
{
	ClearInventory();
	Super::DestroyDueToDeath();
}

void ADodShooter::OnRep_Controller()
{
	Super::OnRep_Controller();
	ChooseViewPerson();
}

void ADodShooter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	ChooseViewPerson();
}

void ADodShooter::Reset()
{
	Super::Reset();
	ClearInventory();
}

void ADodShooter::HandleExperienceLoaded(const UDodExperienceDefinition* CurrentExperience)
{
	CheckForAddInitialInventory();
}

void ADodShooter::CheckForAddInitialInventory()
{
	if (GetController() &&
		GetController()->GetComponentByClass<UDodInventoryManagerComponent>() &&
		GetController()->GetComponentByClass<UDodQuickBarComponent>())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::AddInitialInventory);
		return;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::CheckForAddInitialInventory);
}

void ADodShooter::AddInitialInventory()
{
	if (!HasAuthority())
	{
		return;
	}
	if (!GetController())
	{
		return;
	}

	UDodInventoryManagerComponent* InventoryManager = GetController()->GetComponentByClass<
		UDodInventoryManagerComponent>();
	UDodQuickBarComponent* QuickBar = GetController()->GetComponentByClass<UDodQuickBarComponent>();
	if (!InventoryManager || !QuickBar)
	{
		return;
	}

	for (int32 Idx = 0; Idx < InitialInventoryItems.Num(); ++Idx)
	{
		TSubclassOf<UDodInventoryItemDefinition> ItemDef = InitialInventoryItems[Idx].LoadSynchronous();
		if (IsValid(ItemDef))
		{
			UDodInventoryItemInstance* ItemInstance = InventoryManager->AddItemDefinition(ItemDef, 1);
			QuickBar->AddItemToSlot(Idx, ItemInstance);
		}
	}
	if (InitialInventoryItems.Num() > 0)
	{
		QuickBar->SetActiveSlotIndex(0);
	}
}

void ADodShooter::ClearInventory()
{
	if (HasAuthority() && GetController())
	{
		UDodInventoryManagerComponent* InventoryManager =
			GetController()->GetComponentByClass<UDodInventoryManagerComponent>();
		UDodQuickBarComponent* QuickBar =
			GetController()->GetComponentByClass<UDodQuickBarComponent>();
		if (QuickBar)
		{
			const TArray<UDodInventoryItemInstance*>& Slots = QuickBar->GetSlots();
			for (int32 Idx = 0; Idx < Slots.Num(); ++Idx)
			{
				QuickBar->RemoveItemFromSlot(Idx);
			}
		}
		if (InventoryManager)
		{
			const TArray<UDodInventoryItemInstance*>& Items = InventoryManager->GetAllItems();
			for (UDodInventoryItemInstance* Item : Items)
			{
				InventoryManager->RemoveItemInstance(Item);
			}
		}
	}
}

void ADodShooter::ShowPawnAgainNextFrame(const FActorInitStateChangedParams& Params)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick([&]()
	{
		SetActorHiddenInGame(false);
	});
}
