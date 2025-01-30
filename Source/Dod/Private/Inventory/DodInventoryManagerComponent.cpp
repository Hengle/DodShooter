#include "Inventory/DodInventoryManagerComponent.h"

#include "Engine/ActorChannel.h"
#include "Inventory/DodInventoryItemDefinition.h"
#include "Inventory/DodInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"

TArray<UDodInventoryItemInstance*> FDodInventoryList::GetAllItems() const
{
	TArray<UDodInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FDodInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr)
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

UDodInventoryItemInstance* FDodInventoryList::AddEntry(TSubclassOf<UDodInventoryItemDefinition> ItemDef,
                                                       int32 StackCount)
{
	check(ItemDef);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FDodInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UDodInventoryItemInstance>(OwnerComponent->GetOwner());
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UDodInventoryItemFragment* Fragment : GetDefault<UDodInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	UDodInventoryItemInstance* Result = NewEntry.Instance;

	MarkItemDirty(NewEntry);

	return Result;
}

void FDodInventoryList::RemoveEntry(UDodInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FDodInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

void FDodInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FDodInventoryEntry& Stack = Entries[Index];
		Stack.LastObservedCount = 0;
	}
}

void FDodInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FDodInventoryEntry& Stack = Entries[Index];
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FDodInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FDodInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

UDodInventoryManagerComponent::UDodInventoryManagerComponent(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer), InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void UDodInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

UDodInventoryItemInstance* UDodInventoryManagerComponent::AddItemDefinition(
	TSubclassOf<UDodInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UDodInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);

		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UDodInventoryManagerComponent::RemoveItemInstance(UDodInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UDodInventoryItemInstance*> UDodInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

bool UDodInventoryManagerComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
                                                        FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FDodInventoryEntry& Entry : InventoryList.Entries)
	{
		UDodInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UDodInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (IsUsingRegisteredSubObjectList())
	{
		for (const FDodInventoryEntry& Entry : InventoryList.Entries)
		{
			UDodInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}
