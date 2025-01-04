#include "Inventory/DodInventoryManagerComponent.h"

#include "Inventory/DodInventoryItemDefinition.h"
#include "Inventory/DodInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"

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
