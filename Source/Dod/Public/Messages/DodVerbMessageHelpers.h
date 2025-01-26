#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DodVerbMessageHelpers.generated.h"

UCLASS()
class DOD_API UDodVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Dod")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);
};
