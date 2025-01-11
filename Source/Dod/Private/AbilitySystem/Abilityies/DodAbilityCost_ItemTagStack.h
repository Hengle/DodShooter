#pragma once

#include "CoreMinimal.h"
#include "DodAbilityCost.h"
#include "DodAbilityCost_ItemTagStack.generated.h"

UCLASS(meta=(DisplayName="Item Tag Stack"))
class DOD_API UDodAbilityCost_ItemTagStack : public UDodAbilityCost
{
	GENERATED_BODY()

public:
	UDodAbilityCost_ItemTagStack();
	//~ Begin UDodAbilityCost interface
	virtual bool CheckCost(const UDodGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle,
	                       const FGameplayAbilityActorInfo* ActorInfo,
	                       FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UDodGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle,
	                       const FGameplayAbilityActorInfo* ActorInfo,
	                       const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~ End of UDodAbilityCost interface
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FScalableFloat Quantity{1.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag FailureTag;
};
