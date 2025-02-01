#include "UI/Weapon/DodWeaponUI.h"

#include "Equipment/DodEquipmentManagerComponent.h"
#include "Weapon/DodWeaponInstance.h"

void UDodWeaponUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (UDodEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<UDodEquipmentManagerComponent>())
		{
			if (UDodWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<UDodWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					UDodWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}

void UDodWeaponUI::OnWeaponChanged_Implementation(UDodWeaponInstance* OldWeapon, UDodWeaponInstance* NewWeapon)
{
}
