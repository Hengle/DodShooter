#pragma once

#include "CoreMinimal.h"
#include "DodWidgetFactory.h"
#include "DodWidgetFactory_Class.generated.h"

UCLASS()
class DOD_API UDodWidgetFactory_Class : public UDodWidgetFactory
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<UUserWidget> FindWidgetClassForData_Implementation(const UObject* Data) const override;

protected:
	UPROPERTY(EditAnywhere, Category = ListEntries, meta = (AllowAbstract))
	TMap<TSoftClassPtr<UObject>, TSubclassOf<UUserWidget>> EntryWidgetForClass;
};
