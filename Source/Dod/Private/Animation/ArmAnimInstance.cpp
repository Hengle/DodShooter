#include "Animation/ArmAnimInstance.h"

#include "Character/DodShooter.h"
#include "Equipment/DodEquipmentInstance.h"
#include "Equipment/DodQuickBarComponent.h"
#include "Weapon/WeaponBase.h"

void UArmAnimInstance::UpdateIKBoneInfo()
{
	Super::UpdateIKBoneInfo();

	ADodShooter* Character = Cast<ADodShooter>(TryGetPawnOwner());
	if (!Character)
	{
		return;
	}
	if (!Character->GetController())
	{
		return;
	}
	UDodQuickBarComponent* QuickBar = Character->GetController()->GetComponentByClass<UDodQuickBarComponent>();
	if (!QuickBar)
	{
		return;
	}
	UDodEquipmentInstance* EquipmentInstance = QuickBar->GetEquippedItem();
	if (!EquipmentInstance)
	{
		return;
	}
	if (AWeaponBase* Weapon = Cast<AWeaponBase>(EquipmentInstance->GetSpawnedActor()))
	{
		FTransform ArmTransform = Character->ArmMesh->GetComponentTransform();
		FVector VM_Right_World = Weapon->GetIKBoneLocation_VM_Right();
		VM_Right = ArmTransform.InverseTransformPosition(VM_Right_World);
		FVector VM_Left_World = Weapon->GetIKBoneLocation_VM_Left();
		VM_Left = ArmTransform.InverseTransformPosition(VM_Left_World);

		JointLoc_VM_Left = GetSkelMeshComponent()->GetBoneLocation("VB LeftElbowSocket");
		JointLoc_VM_Right = GetSkelMeshComponent()->GetBoneLocation("VB RightElbowSocket");
	}
}
