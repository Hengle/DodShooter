#pragma once

#include "CoreMinimal.h"
#include "DodTeamInfoBase.h"
#include "DodTeamPublicInfo.generated.h"

UCLASS()
class DOD_API ADodTeamPublicInfo : public ADodTeamInfoBase
{
	GENERATED_BODY()

public:
	ADodTeamPublicInfo();

protected:
	virtual void BeginPlay() override;
};
