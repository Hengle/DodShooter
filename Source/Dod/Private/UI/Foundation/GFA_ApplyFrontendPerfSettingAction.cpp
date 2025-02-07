#include "UI/Foundation/GFA_ApplyFrontendPerfSettingAction.h"

#include "Settings/DodSettingsLocal.h"

int32 UGFA_ApplyFrontendPerfSettingAction::ApplicationCounter = 0;

void UGFA_ApplyFrontendPerfSettingAction::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	ApplicationCounter++;
	if (ApplicationCounter == 1)
	{
		UDodSettingsLocal::Get()->SetShouldUseFrontendPerformanceSettings(true);
	}
}

void UGFA_ApplyFrontendPerfSettingAction::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	ApplicationCounter--;
	check(ApplicationCounter >= 0);

	if (ApplicationCounter == 0)
	{
		UDodSettingsLocal::Get()->SetShouldUseFrontendPerformanceSettings(false);
	}
}
