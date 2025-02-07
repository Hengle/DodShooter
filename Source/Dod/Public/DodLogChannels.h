#pragma once

#include "Logging/LogMacros.h"

DOD_API DECLARE_LOG_CATEGORY_EXTERN(LogDod, Log, All);
DOD_API DECLARE_LOG_CATEGORY_EXTERN(LogDodExperience, Log, All);
DOD_API DECLARE_LOG_CATEGORY_EXTERN(LogDodAbilitySystem, Log, All);
DOD_API DECLARE_LOG_CATEGORY_EXTERN(LogDodTeams, Log, All);

DOD_API FString GetClientServerContextString(UObject* ContextObject = nullptr);