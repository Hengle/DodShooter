#include "UI/Weapon/DodReticleWidgetBase.h"

#include "Inventory/DodInventoryItemInstance.h"
#include "Weapon/DodWeaponInstance.h"

void UDodReticleWidgetBase::InitializeFromWeapon(UDodWeaponInstance* InWeapon)
{
	WeaponInstance = InWeapon;
	InventoryInstance = nullptr;
	if (WeaponInstance)
	{
		InventoryInstance = Cast<UDodInventoryItemInstance>(WeaponInstance->GetInstigator());
	}
	OnWeaponInitialized();
}

void UDodReticleWidgetBase::OnWeaponInitialized_Implementation()
{
}
