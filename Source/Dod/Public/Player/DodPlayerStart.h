#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "DodPlayerStart.generated.h"

UCLASS()
class DOD_API ADodPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	ADodPlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
