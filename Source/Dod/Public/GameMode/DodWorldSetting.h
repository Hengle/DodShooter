#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "DodWorldSetting.generated.h"

class UDodExperienceDefinition;

UCLASS()
class DOD_API ADodWorldSetting : public AWorldSettings
{
	GENERATED_BODY()

public:
	ADodWorldSetting(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category=GameMode)
	TSoftClassPtr<UDodExperienceDefinition> DefaultGameplayExperience;
};
