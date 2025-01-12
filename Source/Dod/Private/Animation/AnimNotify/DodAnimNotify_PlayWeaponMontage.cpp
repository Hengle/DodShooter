#include "Animation/AnimNotify/DodAnimNotify_PlayWeaponMontage.h"

#include "Equipment/DodEquipmentManagerComponent.h"
#include "Weapon/DodWeaponInstance.h"
#include "Weapon/WeaponBase.h"

void UDodAnimNotify_PlayWeaponMontage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAnimMontage* MontageLeader = Cast<UAnimMontage>(Animation);

	if (UDodEquipmentManagerComponent* EMC = MeshComp->GetOwner()->GetComponentByClass<UDodEquipmentManagerComponent>())
	{
		TArray<UDodEquipmentInstance*> Weapons =
			EMC->GetEquipmentInstancesOfType(UDodWeaponInstance::StaticClass());
		if (Weapons.Num() > 0)
		{
			if (AActor* WeaponActor = Weapons[0]->GetSpawnedActor())
			{
				if (AWeaponBase* Weapon = Cast<AWeaponBase>(WeaponActor))
				{
					if (Weapon->VM_Receiver)
					{
						if (UAnimInstance* VM_Anim = Weapon->VM_Receiver->GetAnimInstance())
						{
							VM_Anim->Montage_Play(VM_WeaponMontage);
							VM_Anim->MontageSync_Follow(VM_WeaponMontage, MeshComp->GetAnimInstance(), MontageLeader);
						}
						else
						{
							Weapon->VM_Receiver->PlayAnimation(VM_WeaponMontage, false);
						}
					}
					
					if (Weapon->WM_Receiver)
					{
						if (UAnimInstance* WM_Anim = Weapon->WM_Receiver->GetAnimInstance())
						{
							WM_Anim->Montage_Play(WM_WeaponMontage);
							WM_Anim->MontageSync_Follow(WM_WeaponMontage, MeshComp->GetAnimInstance(), MontageLeader);
						}
						else
						{
							Weapon->WM_Receiver->PlayAnimation(WM_WeaponMontage, false);
						}
					}
				}
			}
		}
	}
}
