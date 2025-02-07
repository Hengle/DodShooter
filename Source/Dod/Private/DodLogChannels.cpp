#include "DodLogChannels.h"

DEFINE_LOG_CATEGORY(LogDod);
DEFINE_LOG_CATEGORY(LogDodExperience);
DEFINE_LOG_CATEGORY(LogDodAbilitySystem);
DEFINE_LOG_CATEGORY(LogDodTeams);

FString GetClientServerContextString(UObject* ContextObject)
{
	ENetRole Role = ROLE_None;

	if (AActor* Actor = Cast<AActor>(ContextObject))
	{
		Role = Actor->GetLocalRole();
	}
	else if (UActorComponent* Component = Cast<UActorComponent>(ContextObject))
	{
		Role = Component->GetOwnerRole();
	}

	if (Role != ROLE_None)
	{
		return (Role == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
	}
	
#if WITH_EDITOR
	if (GIsEditor)
	{
		extern ENGINE_API FString GPlayInEditorContextString;
		return GPlayInEditorContextString;
	}
#endif

	return TEXT("[]");
}
