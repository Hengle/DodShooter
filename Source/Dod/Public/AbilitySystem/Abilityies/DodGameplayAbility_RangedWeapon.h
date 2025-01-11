#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbility.h"
#include "DodGameplayAbility_RangedWeapon.generated.h"

class UDodRangedWeaponInstance;

UCLASS()
class DOD_API UDodGameplayAbility_RangedWeapon : public UDodGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Dod|Ability")
	UDodRangedWeaponInstance* GetWeaponInstance() const;
};
