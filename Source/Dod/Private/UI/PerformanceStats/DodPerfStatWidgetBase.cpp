#include "UI/PerformanceStats/DodPerfStatWidgetBase.h"

#include "Performance/DodPerformanceStatSubsystem.h"

double UDodPerfStatWidgetBase::FetchStatValue()
{
	if (!CachedStatSubsystem)
	{
		if (UWorld* World = GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				CachedStatSubsystem = GameInstance->GetSubsystem<UDodPerformanceStatSubsystem>();
			}
		}
	}

	if (CachedStatSubsystem)
	{
		return CachedStatSubsystem->GetCachedStat(StatToDisplay);
	}
	return 0.0;
}
