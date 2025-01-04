#pragma once

#include "CoreMinimal.h"
#include "Equipment/DodEquipmentInstance.h"
#include "DodWeaponInstance.generated.h"

UCLASS()
class DOD_API UDodWeaponInstance : public UDodEquipmentInstance
{
	GENERATED_BODY()

public:
	virtual void OnEquipped() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	TSoftClassPtr<UAnimInstance> WeaponEquipArmAnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	TSoftClassPtr<UAnimInstance> WeaponEquipBodyAnimInstance;
};
