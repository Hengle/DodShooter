#include "Animation/BodyAnimInstance.h"

#include "Character/DodShooter.h"
#include "Equipment/DodEquipmentInstance.h"
#include "Equipment/DodQuickBarComponent.h"
#include "Weapon/WeaponBase.h"

class UDodQuickBarComponent;

void UBodyAnimInstance::UpdateIKBoneInfo()
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
		FTransform ArmTransform = Character->GetMesh()->GetComponentTransform();
		FVector VM_Right_World = Weapon->GetIKBoneLocation_VM_Right();
		WM_Right = ArmTransform.InverseTransformPosition(VM_Right_World);
		FVector VM_Left_World = Weapon->GetIKBoneLocation_VM_Left();
		WM_Left = ArmTransform.InverseTransformPosition(VM_Left_World);

		JointLoc_WM_Left = GetSkelMeshComponent()->GetBoneLocation("VB LeftElbowSocket");
		JointLoc_WM_Right = GetSkelMeshComponent()->GetBoneLocation("VB RightElbowSocket");

		WM_Right = Weapon->GetIKBoneLocation_WM_Right();
		WM_Left = Weapon->GetIKBoneLocation_WM_Left();
	}
}
