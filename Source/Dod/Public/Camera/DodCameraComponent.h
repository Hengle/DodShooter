#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "DodCameraComponent.generated.h"

class UDodCameraMode;
class UDodCameraModeStack;

UCLASS()
class DOD_API UDodCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UDodCameraComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Dod|Camera")
	static UDodCameraComponent* FindCameraComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<UDodCameraComponent>() : nullptr;
	}

protected:
	//~ Begin UCameraComponent interface
	virtual void OnRegister() override;
	//~ End UCameraComponent interface
};
