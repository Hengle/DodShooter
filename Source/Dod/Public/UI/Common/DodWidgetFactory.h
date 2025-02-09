#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DodWidgetFactory.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class DOD_API UDodWidgetFactory : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<UUserWidget> FindWidgetClassForData(const UObject* Data) const;
};
