#pragma once

#include "CoreMinimal.h"
#include "DodTeamInfoBase.h"
#include "DodTeamPrivateInfo.generated.h"

UCLASS()
class DOD_API ADodTeamPrivateInfo : public ADodTeamInfoBase
{
	GENERATED_BODY()

public:
	ADodTeamPrivateInfo();

protected:
	virtual void BeginPlay() override;
};
