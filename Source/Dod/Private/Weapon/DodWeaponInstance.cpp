#include "Weapon/DodWeaponInstance.h"

#include "Character/DodShooter.h"
#include "Equipment/DodEquipmentDefinition.h"
#include "Weapon/WeaponBase.h"

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

void UDodWeaponInstance::SetAttachment(const TArray<FDodAttachmentInfo>& AttachmentInfos)
{
	if (AWeaponBase* SpawnedWeapon = Cast<AWeaponBase>(SpawnedActor))
	{
		TArray<FDodAttachmentMeshDetail> VM_AttachmentInfos;
		TArray<FDodAttachmentMeshDetail> WM_AttachmentInfos;
		for (const FDodAttachmentInfo& AttInfo : AttachmentInfos)
		{
			VM_AttachmentInfos.Add(AttInfo.AttachmentToSpawn.ViewModel);
			WM_AttachmentInfos.Add(AttInfo.AttachmentToSpawn.WorldModel);
		}
		SpawnedWeapon->SetViewModelAttachment(VM_AttachmentInfos);
		SpawnedWeapon->SetWorldModelAttachment(WM_AttachmentInfos);
	}
}

void UDodWeaponInstance::SetCamo(const FCamoInfo& CamoInfo)
{
	if (AWeaponBase* SpawnedWeapon = Cast<AWeaponBase>(SpawnedActor))
	{
		SpawnedWeapon->SetCamo(CamoInfo);
	}
}

void UDodWeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World);
	TimeLastFired = World->GetTimeSeconds();
}
