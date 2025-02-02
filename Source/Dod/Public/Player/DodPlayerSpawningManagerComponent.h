#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "DodPlayerSpawningManagerComponent.generated.h"


UCLASS()
class DOD_API UDodPlayerSpawningManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UDodPlayerSpawningManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	bool ControllerCanRestart(AController* Player);
};
