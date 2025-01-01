#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"

#include "DodHealthComponent.generated.h"


UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class DOD_API UDodHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UDodHealthComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
};
