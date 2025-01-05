#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "DodPlayerController.generated.h"

class ADodPlayerState;
class UDodAbilitySystemComponent;
class UDodQuickBarComponent;
class UDodInventoryManagerComponent;

UCLASS()
class DOD_API ADodPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

public:
	ADodPlayerController(const FObjectInitializer& FObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Dod|PlayerController")
	ADodPlayerState* GetDodPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Dod|PlayerController")
	UDodAbilitySystemComponent* GetDodAbilitySystemComponent() const;

	//~ Begin APlayerController interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~ End of APlayerController interface

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TObjectPtr<UDodInventoryManagerComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Inventory")
	TObjectPtr<UDodQuickBarComponent> QuickBarComponent;
};
