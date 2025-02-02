#pragma once

#include "CoreMinimal.h"
#include "DodPlayerBotController.h"
#include "AIController_Shooter.generated.h"

class UBehaviorTree;

UCLASS()
class DOD_API AAIController_Shooter : public ADodPlayerBotController
{
	GENERATED_BODY()

public:
	AAIController_Shooter();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void OnWatchedAgentChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

	UFUNCTION()
	void OnDeathStarted(AActor* OwningActor);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	UBehaviorTree* BTAsset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	FName EnemyNameKey;
};
