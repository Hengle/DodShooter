#include "UI/Foundation/DodButtonBase.h"
#include "CommonActionWidget.h"

void UDodButtonBase::SetButtonText(const FText& InText)
{
	bOverride_ButtonText = InText.IsEmpty();
	ButtonText = InText;
	RefreshButtonText();
}

void UDodButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateButtonStyle();
	RefreshButtonText();
}

void UDodButtonBase::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();
	UpdateButtonStyle();
	RefreshButtonText();
}

void UDodButtonBase::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);
	UpdateButtonStyle();
}

void UDodButtonBase::RefreshButtonText()
{
	if (bOverride_ButtonText || ButtonText.IsEmpty())
	{
		if (InputActionWidget)
		{
			const FText ActionDisplayText = InputActionWidget->GetDisplayText();
			if (!ActionDisplayText.IsEmpty())
			{
				UpdateButtonText(ActionDisplayText);
				return;
			}
		}
	}

	UpdateButtonText(ButtonText);
}
