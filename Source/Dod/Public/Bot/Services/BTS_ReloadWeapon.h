#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTS_ReloadWeapon.generated.h"

UCLASS()
class DOD_API UBTS_ReloadWeapon : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
