#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "DodPlayerController.generated.h"

class UDodQuickBarComponent;
class UDodInventoryManagerComponent;

UCLASS()
class DOD_API ADodPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

public:
	ADodPlayerController(const FObjectInitializer& FObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TObjectPtr<UDodInventoryManagerComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TObjectPtr<UDodQuickBarComponent> QuickBarComponent;
};
