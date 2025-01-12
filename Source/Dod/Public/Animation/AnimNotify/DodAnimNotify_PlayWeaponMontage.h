#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DodAnimNotify_PlayWeaponMontage.generated.h"

UCLASS()
class DOD_API UDodAnimNotify_PlayWeaponMontage : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* VM_WeaponMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* WM_WeaponMontage;
};
