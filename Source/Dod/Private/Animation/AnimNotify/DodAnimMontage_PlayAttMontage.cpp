#include "Animation/AnimNotify/DodAnimMontage_PlayAttMontage.h"

#include "Weapon/WeaponBase.h"

void UDodAnimMontage_PlayAttMontage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAnimMontage* MontageLeader = Cast<UAnimMontage>(Animation);

	if (AActor* WeaponActor = MeshComp->GetOwner())
	{
		if (AWeaponBase* Weapon = Cast<AWeaponBase>(WeaponActor))
		{
			if (Weapon->VM_Attachment.IsValidIndex(VM_Slot) && Weapon->VM_Attachment[VM_Slot])
			{
				Weapon->VM_Attachment[VM_Slot]->PlayAnimation(VM_AttMontage, false);
				/*if (UAnimInstance* VM_Anim = Weapon->VM_Attachment[VM_Slot]->GetAnimInstance())
				{
					VM_Anim->Montage_Play(VM_AttMontage);
					VM_Anim->MontageSync_Follow(VM_AttMontage, MeshComp->GetAnimInstance(), MontageLeader);
				}*/
			}

			if (Weapon->WM_Attachment.IsValidIndex(WM_Slot) && Weapon->WM_Attachment[WM_Slot])
			{
				Weapon->WM_Attachment[WM_Slot]->PlayAnimation(WM_AttMontage, false);
				/*if (UAnimInstance* WM_Anim = Weapon->WM_Attachment[WM_Slot]->GetAnimInstance())
				{
					WM_Anim->Montage_Play(WM_AttMontage);
					WM_Anim->MontageSync_Follow(WM_AttMontage, MeshComp->GetAnimInstance(), MontageLeader);
				}*/
			}
		}
	}
}
