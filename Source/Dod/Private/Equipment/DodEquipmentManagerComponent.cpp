#include "Equipment/DodEquipmentManagerComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "Engine/ActorChannel.h"
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

void FDodEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FDodAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
	}
}

void FDodEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FDodAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FDodEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
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

bool UDodEquipmentManagerComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
                                                        FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FDodAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UDodEquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UDodEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UDodEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UDodEquipmentInstance*> AllEquipmentInstances;

	for (const FDodAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UDodEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UDodEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FDodAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UDodEquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

void UDodEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}
