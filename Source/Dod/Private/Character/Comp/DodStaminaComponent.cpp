#include "Character/Comp/DodStaminaComponent.h"

UDodStaminaComponent::UDodStaminaComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UDodStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
}