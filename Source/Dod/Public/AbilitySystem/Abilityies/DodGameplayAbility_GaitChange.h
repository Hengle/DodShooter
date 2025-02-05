#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbilityWithWidget.h"
#include "DodGameplayAbility_GaitChange.generated.h"

/**
 * UDodGameplayAbility_GaitChange
 * 步态切换，使用它管理奔跑和冲刺的能力
 */
UCLASS()
class DOD_API UDodGameplayAbility_GaitChange : public UDodGameplayAbilityWithWidget
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo) override;

	void ActiveSprint();
	void ActiveSuperSprint();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UDodGameplayAbility> SprintAbility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UDodGameplayAbility> SuperSprintAbility;
};
