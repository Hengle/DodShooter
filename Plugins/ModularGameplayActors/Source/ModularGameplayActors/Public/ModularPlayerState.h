#pragma once

#include "GameFramework/PlayerState.h"
#include "ModularPlayerState.generated.h"

UCLASS()
class MODULARGAMEPLAYACTORS_API AModularPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void Reset() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface
};
