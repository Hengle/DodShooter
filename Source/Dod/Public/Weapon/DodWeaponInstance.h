#pragma once

#include "CoreMinimal.h"
#include "Equipment/DodEquipmentInstance.h"
#include "DodWeaponInstance.generated.h"

struct FDodAttachmentInfo;

UCLASS()
class DOD_API UDodWeaponInstance : public UDodEquipmentInstance
{
	GENERATED_BODY()

public:
	virtual void OnEquipped() override;

	void SetAttachment(const TArray<FDodAttachmentInfo>& AttachmentInfos);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	TSoftClassPtr<UAnimInstance> WeaponEquipArmAnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	TSoftClassPtr<UAnimInstance> WeaponEquipBodyAnimInstance;
};
