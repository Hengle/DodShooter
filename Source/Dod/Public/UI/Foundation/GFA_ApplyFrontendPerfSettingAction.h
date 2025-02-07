#pragma once

#include "GameFeatureAction.h"
#include "GFA_ApplyFrontendPerfSettingAction.generated.h"

UCLASS(MinimalAPI, meta = (DisplayName = "Use Frontend Perf Settings"))
class UGFA_ApplyFrontendPerfSettingAction : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	//~UGameFeatureAction interface
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~End of UGameFeatureAction interface

private:
	static int32 ApplicationCounter;
};
