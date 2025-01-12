#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "DodAnimMontage_PlayAttMontage.generated.h"

UCLASS()
class DOD_API UDodAnimMontage_PlayAttMontage : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* VM_AttMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 VM_Slot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* WM_AttMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WM_Slot;
};
