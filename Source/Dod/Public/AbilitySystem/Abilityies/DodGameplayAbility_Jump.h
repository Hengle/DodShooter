#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbility.h"
#include "DodGameplayAbility_Jump.generated.h"

UCLASS()
class DOD_API UDodGameplayAbility_Jump : public UDodGameplayAbility
{
	GENERATED_BODY()

public:
	UDodGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags = nullptr,
	                                const FGameplayTagContainer* TargetTags = nullptr,
	                                FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Dod|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Dod|Ability")
	void CharacterJumpStop();
};
