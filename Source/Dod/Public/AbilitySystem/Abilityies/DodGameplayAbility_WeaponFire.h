#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbility_RangedWeapon.h"
#include "DodGameplayAbility_WeaponFire.generated.h"

UCLASS()
class DOD_API UDodGameplayAbility_WeaponFire : public UDodGameplayAbility_RangedWeapon
{
	GENERATED_BODY()

public:
	//~ Begin UDodGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	//~ End of UDodGameplayAbility interface

protected:
	void SpawnProjectile(FVector FireLocation);

	UFUNCTION(Server, Reliable)
	void Server_SpawnProjectile(FVector FireLocation);

private:
	FTimerHandle FireCompleteTimer;
};
