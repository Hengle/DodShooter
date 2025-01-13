#include "Character/DodShooter.h"

#include "DodGameplayTags.h"
#include "Camera/DodCameraComponent.h"
#include "Character/Comp/DodPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Equipment/DodEquipmentManagerComponent.h"
#include "Equipment/DodQuickBarComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/DodInventoryItemDefinition.h"
#include "Inventory/DodInventoryManagerComponent.h"

const FName ADodShooter::NAME_ActorFeatureName("Shooter");

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

	ensure(TryToChangeInitState(DodGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void ADodShooter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CheckDefaultInitialization();
}

bool ADodShooter::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                     FGameplayTag DesiredState) const
{
	if (CurrentState == DodGameplayTags::InitState_DataInitialized &&
		DesiredState == DodGameplayTags::InitState_GameplayReady)
	{
		if (!GetController() || !GetAbilitySystemComponent())
		{
			return false;
		}
	}
	return true;
}

void ADodShooter::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                        FGameplayTag DesiredState)
{
	if (CurrentState == DodGameplayTags::InitState_DataInitialized &&
		DesiredState == DodGameplayTags::InitState_GameplayReady)
	{
		AddInitialInventory();
		if (IsLocallyControlled())
		{
			ChangeToFirstPerson();
		}
		else
		{
			ChangeToThirdPerson();
		}
	}
}

void ADodShooter::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != UDodPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == DodGameplayTags::InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

void ADodShooter::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = {
		DodGameplayTags::InitState_Spawned,
		DodGameplayTags::InitState_DataAvailable,
		DodGameplayTags::InitState_DataInitialized,
		DodGameplayTags::InitState_GameplayReady
	};

	ContinueInitStateChain(StateChain);
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
