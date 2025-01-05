#include "Player/DodPlayerController.h"

#include "AbilitySystem/DodAbilitySystemComponent.h"
#include "Equipment/DodQuickBarComponent.h"
#include "Inventory/DodInventoryManagerComponent.h"
#include "Player/DodPlayerState.h"

ADodPlayerController::ADodPlayerController(const FObjectInitializer& FObjectInitializer)
	: Super(FObjectInitializer)
{
	InventoryComponent = CreateDefaultSubobject<UDodInventoryManagerComponent>(TEXT("InventoryComponent"));
	QuickBarComponent = CreateDefaultSubobject<UDodQuickBarComponent>(TEXT("QuickBarComponent"));
}

ADodPlayerState* ADodPlayerController::GetDodPlayerState() const
{
	return CastChecked<ADodPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UDodAbilitySystemComponent* ADodPlayerController::GetDodAbilitySystemComponent() const
{
	const ADodPlayerState* PS = GetDodPlayerState();
	return PS ? PS->GetDodAbilitySystemComponent() : nullptr;
}

void ADodPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UDodAbilitySystemComponent* ASC = GetDodAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
