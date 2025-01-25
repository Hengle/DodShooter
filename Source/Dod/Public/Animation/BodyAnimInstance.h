#pragma once

#include "CoreMinimal.h"
#include "DodAnimInstance.h"
#include "BodyAnimInstance.generated.h"

UCLASS()
class DOD_API UBodyAnimInstance : public UDodAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void UpdateIKBoneInfo() override;

	UPROPERTY(BlueprintReadWrite, Category="IKBoneInfo")
	FVector JointLoc_WM_Right{FVector::ZeroVector};
	UPROPERTY(BlueprintReadWrite, Category="IKBoneInfo")
	FVector JointLoc_WM_Left{FVector::ZeroVector};
	UPROPERTY(BlueprintReadWrite, Category="IKBoneInfo")
	FVector WM_Right{FVector::ZeroVector};
	UPROPERTY(BlueprintReadWrite, Category="IKBoneInfo")
	FVector WM_Left{FVector::ZeroVector};
};
