#include "Player/DodPlayerSpawningManagerComponent.h"


UDodPlayerSpawningManagerComponent::UDodPlayerSpawningManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UDodPlayerSpawningManagerComponent::ControllerCanRestart(AController* Player)
{
	return true;
}
