#include "Inventory/DodInventoryItemInstance.h"

#include "Inventory/DodInventoryItemDefinition.h"
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#include "Net/UnrealNetwork.h"

void UDodInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void UDodInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UDodInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UDodInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void UDodInventoryItemInstance::SetItemDef(TSubclassOf<UDodInventoryItemDefinition> InDef)
{
	ItemDef = InDef;
}

const UDodInventoryItemFragment* UDodInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<UDodInventoryItemFragment> FragmentClass) const
{
	if (ItemDef && FragmentClass)
	{
		return GetDefault<UDodInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}

void UDodInventoryItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemDef);
}

void UDodInventoryItemInstance::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
                                                             UE::Net::EFragmentRegistrationFlags RegistrationFlags)
{
	using namespace UE::Net;
	FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
}
