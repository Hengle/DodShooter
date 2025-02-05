#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DodCharacterMovementComp.generated.h"

UCLASS(Blueprintable)
class DOD_API UDodCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UDodCharacterMovementComp();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float GetMaxSpeed() const override;

	void SetCurrentSpeedFactor(float NewSpeedFactor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float WalkSpeed{150.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JogSpeed{250.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SprintSpeed{450.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SuperSprintSpeed{600.f};

	/* 持有武器或者特长的情况下，更改这里的值调整速度 */
	UPROPERTY(BlueprintReadWrite, Replicated)
	float SpeedFactor{1.f};
};
