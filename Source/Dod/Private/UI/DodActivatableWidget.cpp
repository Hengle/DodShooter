#include "UI/DodActivatableWidget.h"

UDodActivatableWidget::UDodActivatableWidget(const FObjectInitializer& ObjectInitializer)
{
}

TOptional<FUIInputConfig> UDodActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EDodWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EDodWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EDodWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	default:
		return TOptional<FUIInputConfig>();
	}
}
