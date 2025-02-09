#include "UI/Common/DodWidgetFactory.h"

TSubclassOf<UUserWidget> UDodWidgetFactory::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	return TSubclassOf<UUserWidget>();
}
