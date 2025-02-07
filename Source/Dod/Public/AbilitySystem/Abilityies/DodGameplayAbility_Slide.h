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
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

protected:
	void GetDirection(FVector& Facing, FVector& LastMovementInput, FVector& Movement);
	bool SelectDirectionalMontage(const FVector& FacingDirection, const FVector& MovementDirection,
	                              UAnimMontage*& BodyAnimMontage);

	void PlayMontage();
	void ApplyRootMotion();

	UFUNCTION(Server, Reliable)
	void Server_SendInfo(FVector Direction, UAnimMontage* BodyMontage);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Slide")
	UAnimMontage* SlideMontage7;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Slide")
	UAnimMontage* SlideMontage8;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Slide")
	UAnimMontage* SlideMontage9;

protected:
	FVector ActorFacing, ActorLastMovementInput, ActorMovement;
	UPROPERTY()
	UAnimMontage* BodyDirMontage;

private:
	uint16 SlideRootMotionSourceID;
};
