#pragma once

#include "CoreMinimal.h"
#include "DodInventoryItemDefinition.h"
#include "IIF_ReticleConfig.generated.h"

class UDodReticleWidgetBase;

UCLASS()
class DOD_API UIIF_ReticleConfig : public UDodInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Reticle")
	TArray<TSubclassOf<UDodReticleWidgetBase>> ReticleWidgets;
};
