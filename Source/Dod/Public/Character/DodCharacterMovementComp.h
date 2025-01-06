#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DodCharacterMovementComp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOD_API UDodCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UDodCharacterMovementComp();
};
