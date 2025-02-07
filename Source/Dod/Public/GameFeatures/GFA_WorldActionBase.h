#pragma once

#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GFA_WorldActionBase.generated.h"

UCLASS(Abstract)
class UGFA_WorldActionBase : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~ End UGameFeatureAction interface
protected:
	void HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext);

	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
	PURE_VIRTUAL(UGameFeatureAction_WorldActionBase::AddToWorld,);

	TMap<FGameFeatureStateChangeContext, FDelegateHandle> GameInstanceStartHandles;
};
