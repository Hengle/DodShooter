#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "DodStaminaComponent.generated.h"


UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DOD_API UDodStaminaComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UDodStaminaComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
};
