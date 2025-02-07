#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Performance/DodPerformanceStatTypes.h"
#include "DodSettingsLocal.generated.h"

UCLASS()
class DOD_API UDodSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UDodSettingsLocal();

	static UDodSettingsLocal* Get();

	//~ Begin UObject interface
	virtual void BeginDestroy() override;
	//~ End UObject interface

	//~ Begin UGameUserSettings interface
	virtual void SetToDefaults() override;
	virtual void LoadSettings(bool bForceReload = false) override;
	//~ End UGameUserSettings interface

	//////////////////////////////////////////////////////////////////
	// Frontend 状态
public:
	void SetShouldUseFrontendPerformanceSettings(bool bInFrontEnd);

protected:
	void UpdateEffectiveFrameRateLimit();

private:
	bool bInFrontEndForPerformancePurposes = false;
	//////////////////////////////////////////////////////////////////
	// 性能监视
public:
	EDodStatDisplayMode GetPerfStatDisplayState(EDodDisplayablePerformanceStat Stat) const;

	void SetPerfStatDisplayState(EDodDisplayablePerformanceStat Stat, EDodStatDisplayMode DisplayMode);
	
	DECLARE_EVENT(UDodSettingsLocal, FPerfStatSettingsChanged);
	FPerfStatSettingsChanged& OnPerfStatDisplayStateChanged() { return PerfStatSettingsChangedEvent; }

private:
	UPROPERTY(Config)
	TMap<EDodDisplayablePerformanceStat, EDodStatDisplayMode> DisplayStatList;
	
	FPerfStatSettingsChanged PerfStatSettingsChangedEvent;

private:
	void OnAppActivationStateChanged(bool bIsActive);

	FDelegateHandle OnApplicationActivationStateChangedHandle;
};
