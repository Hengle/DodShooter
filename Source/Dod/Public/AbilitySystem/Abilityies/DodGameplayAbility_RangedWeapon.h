#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbility.h"
#include "DodGameplayAbility_RangedWeapon.generated.h"

class UDodRangedWeaponInstance;

UENUM(BlueprintType)
enum class EDodAbilityTargetingSource : uint8
{
	// From the player's camera towards camera focus
	CameraTowardsFocus,
	// From the pawn's center, in the pawn's orientation
	PawnForward,
	// From the pawn's center, oriented towards camera focus
	PawnTowardsFocus,
	// From the weapon's muzzle or location, in the pawn's orientation
	WeaponForward,
	// From the weapon's muzzle or location, towards camera focus
	WeaponTowardsFocus,
	// Custom blueprint-specified source location
	Custom
};

UCLASS()
class DOD_API UDodGameplayAbility_RangedWeapon : public UDodGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Dod|Ability")
	UDodRangedWeaponInstance* GetWeaponInstance() const;

	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags = nullptr,
	                                const FGameplayTagContainer* TargetTags = nullptr,
	                                OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	//~End of UGameplayAbility interface

protected:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace{ForceInitToZero};

		FVector EndAim{ForceInitToZero};

		FVector AimDir{ForceInitToZero};

		UDodRangedWeaponInstance* WeaponData = nullptr;

		bool bCanPlayBulletFX = false;
	};

	static int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults);

	FVector GetWeaponTargetingSourceLocation() const;
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);
private:
	FDelegateHandle OnTargetDataReadyCallbackDelegateHandle;
};
