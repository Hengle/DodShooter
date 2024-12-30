#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "CommonLoadingScreenSettings.generated.h"

UCLASS(config=Game, defaultconfig, meta=(DisplayName="Common Loading Screen"))
class LOADINGSCREEN_API UCommonLoadingScreenSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere, Category=Display, meta=(MetaClass="/Script/UMG.UserWidget"))
	FSoftClassPath LoadingScreenWidget;

	UPROPERTY(config, EditAnywhere, Category=Display)
	int32 LoadingScreenZOrder = 10000;
};
