#include "Inventory/DodInventoryManagerComponent.h"

UDodInventoryManagerComponent::UDodInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDodInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDodInventoryManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

