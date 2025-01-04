#include "Weapon/DodWeaponInstance.h"

#include "Character/DodShooter.h"

void UDodWeaponInstance::OnEquipped()
{
	Super::OnEquipped();

	if (ADodShooter* Player = Cast<ADodShooter>(GetPawn()))
	{
		if (TSubclassOf<UAnimInstance> ArmAnim = WeaponEquipArmAnimInstance.LoadSynchronous())
		{
			Player->ArmMesh->LinkAnimClassLayers(ArmAnim);
		}
		if (TSubclassOf<UAnimInstance> BodyAnim = WeaponEquipBodyAnimInstance.LoadSynchronous())
		{
			Player->GetMesh()->LinkAnimClassLayers(BodyAnim);
		}
	}
}
