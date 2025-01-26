#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilityies/DodGameplayAbility.h"
#include "DodGamePhaseAbility.generated.h"

UCLASS(Abstract, HideCategories = Input)
class DOD_API UDodGamePhaseAbility : public UDodGameplayAbility
{
	GENERATED_BODY()

public:
	UDodGamePhaseAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	const FGameplayTag& GetGamePhaseTag() const { return GamePhaseTag; }

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Game Phase")
	FGameplayTag GamePhaseTag;
};
