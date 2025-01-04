#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DodHUD.generated.h"

UCLASS()
class DOD_API ADodHUD : public AHUD
{
	GENERATED_BODY()

protected:
	//~UObject interface
	virtual void PreInitializeComponents() override;
	//~End of UObject interface

	//~AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface
};
