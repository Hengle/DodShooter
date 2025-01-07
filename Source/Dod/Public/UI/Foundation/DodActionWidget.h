#pragma once

#include "CoreMinimal.h"
#include "CommonActionWidget.h"
#include "DodActionWidget.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;

UCLASS(BlueprintType, Blueprintable)
class DOD_API UDodActionWidget : public UCommonActionWidget
{
	GENERATED_BODY()

public:
	//~ Begin UCommonActionWidget interface
	virtual FSlateBrush GetIcon() const override;
	//~ End of UCommonActionWidget interface

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	const TObjectPtr<UInputAction> AssociatedInputAction;

private:
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
};
