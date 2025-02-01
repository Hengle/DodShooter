#pragma once

#include "CoreMinimal.h"
#include "Messages/GameplayMessageProcessor.h"
#include "EliminationFeedRelay.generated.h"


struct FDodVerbMessage;
struct FGameplayTag;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOD_API UEliminationFeedRelay : public UGameplayMessageProcessor
{
	GENERATED_BODY()

public:
	UEliminationFeedRelay();

	void ReceivedElimination(FGameplayTag Channel, const FDodVerbMessage& Payload);

protected:
	virtual void BeginPlay() override;
};
