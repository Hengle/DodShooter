#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ModularPlayerController.generated.h"

UCLASS()
class MODULARGAMEPLAYACTORS_API AModularPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface

	//~ Begin APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~ End APlayerController interface
};
