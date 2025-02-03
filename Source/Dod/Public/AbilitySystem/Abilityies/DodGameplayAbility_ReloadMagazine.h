#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbility.h"
#include "DodGameplayAbility_ReloadMagazine.generated.h"

UCLASS()
class DOD_API UDodGameplayAbility_ReloadMagazine : public UDodGameplayAbility
{
	GENERATED_BODY()

public:
	UDodGameplayAbility_ReloadMagazine(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

protected:
	void ReloadAmmoIntoMagazine();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* BodyReloadMontage{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ArmReloadMontage{nullptr};

	uint8 bDidBlockFiring : 1{false};

private:
	FTimerHandle ReloadAddTimerHandle;
	FTimerHandle ReloadTimerHandle;
};
