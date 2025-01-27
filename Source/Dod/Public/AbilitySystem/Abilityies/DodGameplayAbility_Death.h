#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbility.h"
#include "DodGameplayAbility_Death.generated.h"

UCLASS()
class DOD_API UDodGameplayAbility_Death : public UDodGameplayAbility
{
	GENERATED_BODY()

public:
	UDodGameplayAbility_Death(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable, Category = "Dod|Ability")
	void StartDeath();
	UFUNCTION(BlueprintCallable, Category = "Dod|Ability")
	void FinishDeath();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Death")
	bool bAutoStartDeath;
};
