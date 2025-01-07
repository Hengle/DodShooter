#pragma once

#include "Input/CommonBoundActionButton.h"
#include "DodBoundActionButton.generated.h"

UCLASS(Abstract, meta = (DisableNativeTick))
class DOD_API UDodBoundActionButton : public UCommonBoundActionButton
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	void HandleInputMethodChanged(ECommonInputType NewInputMethod);

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> KeyboardStyle;

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> GamepadStyle;

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> TouchStyle;
};
