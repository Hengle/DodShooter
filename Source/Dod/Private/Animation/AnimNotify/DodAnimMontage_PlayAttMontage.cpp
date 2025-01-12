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
			for (const auto& [AttMontage, AttSlot] : VM_AttMontages)
			{
				if (Weapon->VM_Attachment.IsValidIndex(AttSlot) && Weapon->VM_Attachment[AttSlot])
				{
					if (UAnimInstance* VM_Anim = Weapon->VM_Attachment[AttSlot]->GetAnimInstance())
					{
						VM_Anim->Montage_Play(AttMontage);
						VM_Anim->MontageSync_Follow(AttMontage, MeshComp->GetAnimInstance(), MontageLeader);
					}
					else
					{
						Weapon->VM_Attachment[AttSlot]->PlayAnimation(AttMontage, false);
					}
				}
			}

			for (const auto& [AttMontage, AttSlot] : WM_AttMontages)
			{
				if (Weapon->WM_Attachment.IsValidIndex(AttSlot) && Weapon->WM_Attachment[AttSlot])
				{
					if (UAnimInstance* WM_Anim = Weapon->WM_Attachment[AttSlot]->GetAnimInstance())
					{
						WM_Anim->Montage_Play(AttMontage);
						WM_Anim->MontageSync_Follow(AttMontage, MeshComp->GetAnimInstance(), MontageLeader);
					}
					else
					{
						Weapon->WM_Attachment[AttSlot]->PlayAnimation(AttMontage, false);
					}
				}
			}
		}
	}
}
