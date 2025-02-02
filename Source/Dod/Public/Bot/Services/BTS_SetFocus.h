#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTS_SetFocus.generated.h"

UCLASS()
class DOD_API UBTS_SetFocus : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector TargetEnemy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 bSetFocus : 1{false};
};
