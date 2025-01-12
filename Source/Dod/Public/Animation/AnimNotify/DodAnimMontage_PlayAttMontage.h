#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "DodAnimMontage_PlayAttMontage.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FAttMontageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttSlot;
};

UCLASS()
class DOD_API UDodAnimMontage_PlayAttMontage : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAttMontageInfo> VM_AttMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAttMontageInfo> WM_AttMontages;
};
