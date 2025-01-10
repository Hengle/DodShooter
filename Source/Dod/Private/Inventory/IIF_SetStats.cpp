#include "Inventory/IIF_SetStats.h"

#include "Inventory/DodInventoryItemInstance.h"

void UIIF_SetStats::OnInstanceCreated(UDodInventoryItemInstance* Instance) const
{
	for (const auto& ItemStat : InitialItemStats)
	{
		Instance->AddStatTagStack(ItemStat.Key, ItemStat.Value);
	}
}
