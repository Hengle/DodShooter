#include "Weapon/WeaponBase.h"

#include "GameFramework/Character.h"


AWeaponBase::AWeaponBase()
{
	SetReplicates(true);

	VM_Receiver = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VM_Receiver"));
	VM_Receiver->SetCastShadow(false);
	WM_Receiver = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WM_Receiver"));
	VM_Receiver->SetCastShadow(true);
	VM_Receiver->SetCastHiddenShadow(true);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Player = Cast<ACharacter>(GetOwner()))
	{
		if (!Player->IsLocallyControlled() || Player->IsBotControlled())
		{
			VM_Receiver->SetVisibility(false);
		}
		else
		{
			WM_Receiver->SetVisibility(false);
		}
	}
}
