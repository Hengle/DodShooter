#include "Character/Comp/DodHealthComponent.h"


UDodHealthComponent::UDodHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UDodHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}
