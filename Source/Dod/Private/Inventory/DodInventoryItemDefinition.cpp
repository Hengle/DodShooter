#include "Inventory/DodInventoryItemDefinition.h"

const UDodInventoryItemFragment* UDodInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UDodInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (const UDodInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}
