#pragma once

#include "CoreMinimal.h"
#include "LoadingProcessInterface.h"
#include "Components/GameStateComponent.h"
#include "DodExperienceManagerComponent.generated.h"


namespace UE::GameFeatures
{
	struct FResult;
}

class UDodExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDodExperienceLoaded, const UDodExperienceDefinition* /*Experience*/);

enum class EDodExperienceLoadState : uint8
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

UCLASS()
class DOD_API UDodExperienceManagerComponent : public UGameStateComponent, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	UDodExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin UActorComponent interface
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End of UActorComponent interface

	//~ Begin ILoadingProcessInterface interface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~ End ILoadingProcessInterface interface

	void SetCurrentExperience(FPrimaryAssetId ExperienceId);
	const UDodExperienceDefinition* GetCurrentExperienceChecked() const;
	
	bool IsExperienceLoaded() const;

	void CallOrRegister_OnExperienceLoaded_HighPriority(FOnDodExperienceLoaded::FDelegate&& Delegate);
	void CallOrRegister_OnExperienceLoaded(FOnDodExperienceLoaded::FDelegate&& Delegate);
	void CallOrRegister_OnExperienceLoaded_LowPriority(FOnDodExperienceLoaded::FDelegate&& Delegate);

private:
	UFUNCTION()
	void OnRep_CurrentExperience();

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();

	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();

	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperience)
	TObjectPtr<const UDodExperienceDefinition> CurrentExperience;

	EDodExperienceLoadState LoadState{EDodExperienceLoadState::Unloaded};

	TArray<FString> GameFeaturePluginURLs;
	int32 NumGameFeaturePluginsLoading = 0;

	int32 NumObservedPausers = 0;
	int32 NumExpectedPausers = 0;

	FOnDodExperienceLoaded OnExperienceLoaded_HighPriority;

	/** Delegate called when the experience has finished loading */
	FOnDodExperienceLoaded OnExperienceLoaded;

	/** Delegate called when the experience has finished loading */
	FOnDodExperienceLoaded OnExperienceLoaded_LowPriority;
};
