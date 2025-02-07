#pragma once

#include "GFA_WorldActionBase.h"
#include "UObject/ObjectKey.h"
#include "GFA_SplitscreenConfig.generated.h"

UCLASS(MinimalAPI, meta = (DisplayName = "Splitscreen Config"))
class UGFA_SplitscreenConfig : public UGFA_WorldActionBase
{
	GENERATED_BODY()

public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~ End UGameFeatureAction interface

	//~ Begin UGameFeatureAction_WorldActionBase interface
	virtual void AddToWorld(const FWorldContext& WorldContext,
	                        const FGameFeatureStateChangeContext& ChangeContext) override;
	//~ End UGameFeatureAction_WorldActionBase interface

	UPROPERTY(EditAnywhere, Category=Action)
	bool bDisableSplitscreen = true;

private:
	TArray<FObjectKey> LocalDisableVotes;

	static TMap<FObjectKey, int32> GlobalDisableVotes;
};
