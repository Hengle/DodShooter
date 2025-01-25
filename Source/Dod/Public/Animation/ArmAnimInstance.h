#pragma once

#include "CoreMinimal.h"
#include "DodAnimInstance.h"
#include "ArmAnimInstance.generated.h"

UCLASS()
class DOD_API UArmAnimInstance : public UDodAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void UpdateIKBoneInfo() override;

	UPROPERTY(BlueprintReadWrite, Category="IKBoneInfo")
	FVector JointLoc_VM_Right{FVector::ZeroVector};
	UPROPERTY(BlueprintReadWrite, Category="IKBoneInfo")
	FVector JointLoc_VM_Left{FVector::ZeroVector};
	UPROPERTY(BlueprintReadWrite, Category="IKBoneInfo")
	FVector VM_Right{FVector::ZeroVector};
	UPROPERTY(BlueprintReadWrite, Category="IKBoneInfo")
	FVector VM_Left{FVector::ZeroVector};
};
