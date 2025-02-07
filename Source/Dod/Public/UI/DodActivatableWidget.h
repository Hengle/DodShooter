#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "DodActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class EDodWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

UCLASS(Abstract, Blueprintable)
class DOD_API UDodActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UDodActivatableWidget(const FObjectInitializer& ObjectInitializer);

	//~ Begin UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~ End UCommonActivatableWidget interface
protected:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EDodWidgetInputMode InputConfig{EDodWidgetInputMode::Default};

	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode{EMouseCaptureMode::CapturePermanently};
};
