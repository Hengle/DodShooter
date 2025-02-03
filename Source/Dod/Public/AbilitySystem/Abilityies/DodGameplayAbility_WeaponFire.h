#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbility_RangedWeapon.h"
#include "DodGameplayAbility_WeaponFire.generated.h"

UCLASS()
class DOD_API UDodGameplayAbility_WeaponFire : public UDodGameplayAbility_RangedWeapon
{
	GENERATED_BODY()

public:
	UDodGameplayAbility_WeaponFire(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin UDodGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	virtual void OnAbilityAdded() override;
	//~ End of UDodGameplayAbility interface

	virtual void OnWeaponFireCompleted_Implementation() override;

protected:
	void StartFire();

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAnimMontage> ArmFireMontage;
	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayCueTagFiring;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> FieldActorToSpawnOnImpact;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GE_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireDelayTimeSecs{.12f};

private:
	FGameplayCueParameters GCNParameter;
	FTimerHandle EndTimer;
};
