#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonNumericTextBlock.h"
#include "HealthBar.generated.h"

class UDodHealthComponent;
class UProgressBar;

UCLASS()
class DOD_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	UFUNCTION()
	void HealthChanged(UDodHealthComponent* HealthComponent, float OldValue, float NewValue, AActor* Instigator);

	UPROPERTY(meta=(BindWidget))
	UProgressBar* BarFill;
	UPROPERTY(meta=(BindWidget))
	UCommonNumericTextBlock* HealthValue;
};
