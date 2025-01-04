#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DodGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EDodAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnSpawn
};

UCLASS()
class DOD_API UDodGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class UDodAbilitySystemComponent;

protected:
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	virtual void OnPawnAvatarSet();

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Ability Activation")
	EDodAbilityActivationPolicy ActivationPolicy;
};
