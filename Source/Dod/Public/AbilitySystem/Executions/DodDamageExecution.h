﻿#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "DodDamageExecution.generated.h"

UCLASS()
class DOD_API UDodDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UDodDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
