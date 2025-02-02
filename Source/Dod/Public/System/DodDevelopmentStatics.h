#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DodDevelopmentStatics.generated.h"

UCLASS()
class DOD_API UDodDevelopmentStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Dod")
	static bool CanPlayerBotsAttack();
};
