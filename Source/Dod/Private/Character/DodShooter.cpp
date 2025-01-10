#include "Character/DodShooter.h"

#include "Camera/DodCameraComponent.h"
#include "Equipment/DodEquipmentManagerComponent.h"
#include "Equipment/DodQuickBarComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/DodInventoryItemDefinition.h"
#include "Inventory/DodInventoryManagerComponent.h"


ADodShooter::ADodShooter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(SpringArmComponent);

	CameraComponent->SetupAttachment(ArmMesh, TEXT("tag_camera"));

	EquipmentManager = CreateDefaultSubobject<UDodEquipmentManagerComponent>(TEXT("EquipmentManager"));
}

void ADodShooter::ChangeToFirstPerson()
{
	if (!IsLocallyControlled())
	{
		return;
	}
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

void ADodShooter::BeginPlay()
{
	Super::BeginPlay();

	HeadMesh->SetCastShadow(true);
	HeadMesh->SetCastHiddenShadow(true);
	GetMesh()->SetCastShadow(true);
	GetMesh()->SetCastHiddenShadow(true);
	ArmMesh->SetCastShadow(false);

	if (HasAuthority())
	{
		AddInitialInventory();
	}

	if (IsLocallyControlled())
	{
		ChangeToFirstPerson();
	}
	else
	{
		ChangeToThirdPerson();
	}
}

void ADodShooter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
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

	UDodInventoryManagerComponent* InventoryManager = GetController()->GetComponentByClass<UDodInventoryManagerComponent>();
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
