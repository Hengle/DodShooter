#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UIMessaging.generated.h"

USTRUCT(BlueprintType)
struct FUIMessaging
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	bool bOn;
	UPROPERTY(BlueprintReadWrite)
	APlayerController* Controller;
};
