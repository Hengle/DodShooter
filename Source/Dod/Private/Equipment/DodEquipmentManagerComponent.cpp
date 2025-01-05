#include "Equipment/DodEquipmentManagerComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "Equipment/DodEquipmentDefinition.h"
#include "Equipment/DodEquipmentInstance.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/DodWeaponInstance.h"

UDodEquipmentInstance* FDodEquipmentList::AddEntry(TSubclassOf<UDodEquipmentDefinition> EquipmentDefinition)
{
	check(EquipmentDefinition);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const UDodEquipmentDefinition* EquipmentCDO = GetDefault<UDodEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UDodEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UDodEquipmentInstance::StaticClass();
	}

	FDodAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UDodEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	UDodEquipmentInstance* Result = NewEntry.Instance;

	if (UDodAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TObjectPtr<const UDodAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
		}
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorToSpawn);

	if (UDodWeaponInstance* WeaponInstance = Cast<UDodWeaponInstance>(Result))
	{
		WeaponInstance->SetAttachment(EquipmentCDO->Attachments);
	}

	MarkItemDirty(NewEntry);

	return Result;
}

void FDodEquipmentList::RemoveEntry(UDodEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FDodAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UDodAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();


			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UDodAbilitySystemComponent* FDodEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UDodAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UDodEquipmentManagerComponent::UDodEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
}

UDodEquipmentInstance* UDodEquipmentManagerComponent::EquipItem(
	TSubclassOf<UDodEquipmentDefinition> EquipmentClass)
{
	UDodEquipmentInstance* Result = nullptr;
	if (EquipmentClass)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

void UDodEquipmentManagerComponent::UnequipItem(UDodEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

void UDodEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}
