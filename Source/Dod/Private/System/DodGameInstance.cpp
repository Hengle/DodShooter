#include "System/DodGameInstance.h"

#include "DodGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

void UDodGameInstance::Init()
{
	Super::Init();

	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	if (ensure(ComponentManager))
	{
		ComponentManager->RegisterInitState(DodGameplayTags::InitState_Spawned,
		                                    false,
		                                    FGameplayTag());
		ComponentManager->RegisterInitState(DodGameplayTags::InitState_DataAvailable,
		                                    false,
		                                    DodGameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(DodGameplayTags::InitState_DataInitialized,
		                                    false,
		                                    DodGameplayTags::InitState_DataAvailable);
		ComponentManager->RegisterInitState(DodGameplayTags::InitState_GameplayReady,
		                                    false,
		                                    DodGameplayTags::InitState_DataInitialized);
	}
}
