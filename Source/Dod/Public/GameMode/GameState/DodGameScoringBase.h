#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "DodGameScoringBase.generated.h"


struct FDodVerbMessage;
struct FGameplayTag;

UCLASS(meta=(BlueprintSpawnableComponent))
class DOD_API UDodGameScoringBase : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UDodGameScoringBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

	virtual void OnEliminationScored();

	UFUNCTION()
	void OnReceiveElimination(FGameplayTag Channel, const FDodVerbMessage& Payload);
	UFUNCTION()
	void OnReceiveAssist(FGameplayTag Channel, const FDodVerbMessage& Payload);
};
