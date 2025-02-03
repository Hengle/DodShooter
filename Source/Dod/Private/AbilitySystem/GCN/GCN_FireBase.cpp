#include "AbilitySystem/GCN/GCN_FireBase.h"

#include "Equipment/DodEquipmentManagerComponent.h"
#include "Weapon/DodWeaponInstance.h"
#include "Weapon/WeaponBase.h"

bool UGCN_FireBase::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	AWeaponBase* Weapon = GetWeapon(MyTarget);
	Weapon->TriggerFireAudio(FireSound, MyTarget);

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

AWeaponBase* UGCN_FireBase::GetWeapon(AActor* InActor) const
{
	UDodEquipmentManagerComponent* EMC = InActor->GetComponentByClass<UDodEquipmentManagerComponent>();
	if (!EMC)
	{
		return nullptr;
	}
	TArray<UDodEquipmentInstance*> Weapons = EMC->GetEquipmentInstancesOfType(UDodWeaponInstance::StaticClass());
	if (Weapons.Num() > 0)
	{
		return Cast<AWeaponBase>(Weapons[0]->GetSpawnedActor());
	}
	return nullptr;
}
