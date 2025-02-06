#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbilityWithWidget.h"
#include "DodGameplayAbility_Slide.generated.h"

UCLASS()
class DOD_API UDodGameplayAbility_Slide : public UDodGameplayAbilityWithWidget
{
	GENERATED_BODY()

public:
	UDodGameplayAbility_Slide();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	void GetDirection(FVector& Facing, FVector& LastMovementInput, FVector& Movement);
};
