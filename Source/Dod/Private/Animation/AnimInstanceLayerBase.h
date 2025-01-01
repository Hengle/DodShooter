#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceLayerBase.generated.h"

class UDodAnimInstance;

UCLASS()
class DOD_API UAnimInstanceLayerBase : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	const UDodAnimInstance* GetDodAnimInstance() const;
};
