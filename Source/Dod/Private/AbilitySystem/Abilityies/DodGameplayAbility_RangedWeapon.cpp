#include "AbilitySystem/Abilityies/DodGameplayAbility_RangedWeapon.h"

#include "Weapon/DodRangedWeaponInstance.h"

UDodRangedWeaponInstance* UDodGameplayAbility_RangedWeapon::GetWeaponInstance() const
{
	return Cast<UDodRangedWeaponInstance>(GetAssociatedEquipment());
}
