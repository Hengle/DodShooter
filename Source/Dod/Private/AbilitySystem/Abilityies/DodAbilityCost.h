#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "UObject/Object.h"
#include "DodAbilityCost.generated.h"

struct FGameplayTagContainer;
struct FGameplayAbilityActorInfo;
class UDodGameplayAbility;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class DOD_API UDodAbilityCost : public UObject
{
	GENERATED_BODY()

public:
	virtual bool CheckCost(const UDodGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle,
	                       const FGameplayAbilityActorInfo* ActorInfo,
	                       FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}

	virtual void ApplyCost(const UDodGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle,
	                       const FGameplayAbilityActorInfo* ActorInfo,
	                       const FGameplayAbilityActivationInfo ActivationInfo)
	{
	}

	bool ShouldOnlyApplyCostOnHit() const { return bOnlyApplyCostOnHit; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	bool bOnlyApplyCostOnHit = false;
};
