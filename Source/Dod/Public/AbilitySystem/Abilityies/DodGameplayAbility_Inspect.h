#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbilityWithWidget.h"
#include "DodGameplayAbility_Inspect.generated.h"

UCLASS()
class DOD_API UDodGameplayAbility_Inspect : public UDodGameplayAbilityWithWidget
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	UFUNCTION()
	void CheckForMovement(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* VM_InspectMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* WM_InspectMontage;

private:
	FTimerHandle AnimTimer;
};
