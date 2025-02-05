#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbilityWithWidget.h"
#include "DodGameplayAbility_Sprint.generated.h"

/* UDodGameplayAbility_Sprint
 *
 */
UCLASS()
class DOD_API UDodGameplayAbility_Sprint : public UDodGameplayAbilityWithWidget
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

protected:
	void OnStaminaCostTick();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AddTagToPlayer;

private:
	FTimerHandle StaminaCostTimerHandle;
};
