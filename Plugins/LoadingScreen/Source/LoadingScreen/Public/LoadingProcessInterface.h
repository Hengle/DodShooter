#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LoadingProcessInterface.generated.h"

UINTERFACE()
class ULoadingProcessInterface : public UInterface
{
	GENERATED_BODY()
};

class LOADINGSCREEN_API ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	static bool ShouldShowLoadingScreen(UObject* TestObject);

	virtual bool ShouldShowLoadingScreen() const
	{
		return false;
	}
};
