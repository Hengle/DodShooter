#include "Player/DodPlayerController.h"

#include "Equipment/DodQuickBarComponent.h"
#include "Inventory/DodInventoryManagerComponent.h"

ADodPlayerController::ADodPlayerController(const FObjectInitializer& FObjectInitializer)
	: Super(FObjectInitializer)
{
	InventoryComponent = CreateDefaultSubobject<UDodInventoryManagerComponent>(TEXT("InventoryComponent"));
	QuickBarComponent = CreateDefaultSubobject<UDodQuickBarComponent>(TEXT("QuickBarComponent"));
}
