#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "DodGameInstance.generated.h"

UCLASS()
class DOD_API UDodGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
};
