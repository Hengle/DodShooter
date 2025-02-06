#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "StaminaBar.generated.h"

class UProgressBar;
class UDodStaminaComponent;

UCLASS()
class DOD_API UStaminaBar : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	UFUNCTION()
	void StaminaChanged(UDodStaminaComponent* StaminaComponent, float OldValue, float NewValue, AActor* Instigator);

	UPROPERTY(meta=(BindWidget))
	UProgressBar* BarFill;
};
