#include "LoadingProcessInterface.h"

bool ILoadingProcessInterface::ShouldShowLoadingScreen(UObject* TestObject)
{
	if (TestObject != nullptr)
	{
		if (ILoadingProcessInterface* LoadObserver = Cast<ILoadingProcessInterface>(TestObject))
		{
			if (LoadObserver->ShouldShowLoadingScreen())
			{
				return true;
			}
		}
	}

	return false;
}
