#include "Settings/DodSettingsLocal.h"

UDodSettingsLocal::UDodSettingsLocal()
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && FSlateApplication::IsInitialized())
	{
		OnApplicationActivationStateChangedHandle = FSlateApplication::Get().OnApplicationActivationStateChanged().
		                                                                     AddUObject(
			                                                                     this,
			                                                                     &ThisClass::OnAppActivationStateChanged);
	}

	SetToDefaults();
}

UDodSettingsLocal* UDodSettingsLocal::Get()
{
	return GEngine ? CastChecked<UDodSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
}

void UDodSettingsLocal::BeginDestroy()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().OnApplicationActivationStateChanged().
		                         Remove(OnApplicationActivationStateChangedHandle);
	}

	Super::BeginDestroy();
}

void UDodSettingsLocal::SetToDefaults()
{
	Super::SetToDefaults();

	/*bUseHeadphoneMode = false;
	bUseHDRAudioMode = false;
	bSoundControlBusMixLoaded = false;

	const UDodPlatformSpecificRenderingSettings* PlatformSettings = UDodPlatformSpecificRenderingSettings::Get();
	UserChosenDeviceProfileSuffix = PlatformSettings->DefaultDeviceProfileSuffix;
	DesiredUserChosenDeviceProfileSuffix = UserChosenDeviceProfileSuffix;

	FrameRateLimit_InMenu = 144.0f;
	FrameRateLimit_WhenBackgrounded = 30.0f;
	FrameRateLimit_OnBattery = 60.0f;

	MobileFrameRateLimit = GetDefaultMobileFrameRate();
	DesiredMobileFrameRateLimit = MobileFrameRateLimit;*/
}

void UDodSettingsLocal::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);

	/*const UDodPlatformSpecificRenderingSettings* PlatformSettings = UDodPlatformSpecificRenderingSettings::Get();
	if (PlatformSettings->FramePacingMode == EDodFramePacingMode::ConsoleStyle)
	{
		FrameRateLimit = 0.0f;
	}

	// Enable HRTF if needed
	bDesiredHeadphoneMode = bUseHeadphoneMode;
	SetHeadphoneModeEnabled(bUseHeadphoneMode);


	DesiredUserChosenDeviceProfileSuffix = UserChosenDeviceProfileSuffix;

	DodSettingsHelpers::FillScalabilitySettingsFromDeviceProfile(DeviceDefaultScalabilitySettings);

	DesiredMobileFrameRateLimit = MobileFrameRateLimit;
	ClampMobileQuality();*/

	PerfStatSettingsChangedEvent.Broadcast();
}

void UDodSettingsLocal::SetShouldUseFrontendPerformanceSettings(bool bInFrontEnd)
{
	bInFrontEndForPerformancePurposes = bInFrontEnd;
	UpdateEffectiveFrameRateLimit();
}

void UDodSettingsLocal::UpdateEffectiveFrameRateLimit()
{
	if (!IsRunningDedicatedServer())
	{
		SetFrameRateLimitCVar(GetEffectiveFrameRateLimit());
	}
}

EDodStatDisplayMode UDodSettingsLocal::GetPerfStatDisplayState(EDodDisplayablePerformanceStat Stat) const
{
	if (const EDodStatDisplayMode* pMode = DisplayStatList.Find(Stat))
	{
		return *pMode;
	}
	return EDodStatDisplayMode::Hidden;
}

void UDodSettingsLocal::SetPerfStatDisplayState(EDodDisplayablePerformanceStat Stat, EDodStatDisplayMode DisplayMode)
{
	if (DisplayMode == EDodStatDisplayMode::Hidden)
	{
		DisplayStatList.Remove(Stat);
	}
	else
	{
		DisplayStatList.FindOrAdd(Stat) = DisplayMode;
	}
	PerfStatSettingsChangedEvent.Broadcast();
}

void UDodSettingsLocal::OnAppActivationStateChanged(bool bIsActive)
{
	UpdateEffectiveFrameRateLimit();
}
