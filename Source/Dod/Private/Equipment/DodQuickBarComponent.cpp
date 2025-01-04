#include "Equipment/DodQuickBarComponent.h"

#include "Equipment/DodEquipmentDefinition.h"
#include "Equipment/DodEquipmentInstance.h"
#include "Equipment/DodEquipmentManagerComponent.h"
#include "Inventory/DodInventoryItemInstance.h"
#include "Inventory/IIF_EquippableItem.h"
#include "Net/UnrealNetwork.h"


UDodQuickBarComponent::UDodQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);

	IsValidSlotIndex(0);
}

void UDodQuickBarComponent::AddItemToSlot(int32 SlotIndex, UDodInventoryItemInstance* Item)
{
	if (IsValidSlotIndex(SlotIndex) && Item)
	{
		if (!Slots[SlotIndex])
		{
			Slots[SlotIndex] = Item;
			OnRep_Slots();
		}
	}
}

void UDodQuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();

		ActiveSlotIndex = NewIndex;

		EquipItemInSlot();

		OnRep_ActiveSlotIndex();
	}
}

bool UDodQuickBarComponent::IsValidSlotIndex(int32 SlotIndex)
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
	return Slots.IsValidIndex(SlotIndex);
}

void UDodQuickBarComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDodQuickBarComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
}

void UDodQuickBarComponent::UnequipItemInSlot()
{
	if (UDodEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

void UDodQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(!EquippedItem);

	if (UDodInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		if (const UIIF_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UIIF_EquippableItem>())
		{
			if (const TSubclassOf<UDodEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition)
			{
				if (UDodEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem)
					{
						EquippedItem->SetInstigator(SlotItem);
					}
				}
			}
		}
	}
}

UDodEquipmentManagerComponent* UDodQuickBarComponent::FindEquipmentManager() const
{
	if (const AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UDodEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void UDodQuickBarComponent::OnRep_Slots()
{
}

void UDodQuickBarComponent::OnRep_ActiveSlotIndex()
{
}
