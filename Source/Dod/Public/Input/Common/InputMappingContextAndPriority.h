#pragma once

#include "InputMappingContextAndPriority.generated.h"

class UInputMappingContext;

USTRUCT()
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Input", meta=(AssetBundles="Client,Server"))
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category="Input")
	int32 Priority = 0;

	UPROPERTY(EditAnywhere, Category="Input")
	bool bRegisterWithSettings = true;
};
