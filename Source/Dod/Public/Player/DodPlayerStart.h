#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "DodPlayerStart.generated.h"

enum class EDodPlayerStartLocationOccupancy : uint8
{
	Empty,
	Partial,
	Full
};

UCLASS()
class DOD_API ADodPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	ADodPlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	EDodPlayerStartLocationOccupancy GetLocationOccupancy(AController* const ControllerPawnToFit) const;

	bool IsClaimed() const;
	bool TryClaim(AController* OccupyingController);

protected:
	void CheckUnclaimed();
	
	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player Start Claiming")
	float ExpirationCheckInterval = 1.f;

	FTimerHandle ExpirationTimerHandle;
};
