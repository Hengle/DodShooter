#include "UI/Common/DodWidgetFactory_Class.h"

#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UDodWidgetFactory_Class::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	for (UClass* Class = Data->GetClass(); Class; Class = Class->GetSuperClass())
	{
		TSoftClassPtr<UObject> ClassPtr(Class);
		if (const TSubclassOf<UUserWidget> EntryWidgetClassPtr = EntryWidgetForClass.FindRef(ClassPtr))
		{
			return EntryWidgetClassPtr;
		}
	}

	return TSubclassOf<UUserWidget>();
}
