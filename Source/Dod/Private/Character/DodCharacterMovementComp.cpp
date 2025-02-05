#include "Character/DodCharacterMovementComp.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "DodGameplayTags.h"
#include "Net/UnrealNetwork.h"


UDodCharacterMovementComp::UDodCharacterMovementComp()
{
}

void UDodCharacterMovementComp::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SpeedFactor);
}

float UDodCharacterMovementComp::GetMaxSpeed() const
{
	float BaseSpeed = Super::GetMaxSpeed();

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		// 角色可能同时有四个Tag，按激活顺序和按键顺序的考虑
		// 冲刺>奔跑>静步>小跑(默认)
		if (ASC->HasMatchingGameplayTag(DodGameplayTags::Movement_Gait_SuperSprint))
		{
			BaseSpeed = SuperSprintSpeed;
		}
		else if (ASC->HasMatchingGameplayTag(DodGameplayTags::Movement_Gait_Sprint))
		{
			BaseSpeed = SprintSpeed;
		}
		else if (ASC->HasMatchingGameplayTag(DodGameplayTags::Movement_Gait_Walk))
		{
			BaseSpeed = WalkSpeed;
		}
		else
		{
			BaseSpeed = JogSpeed;
		}
	}

	return BaseSpeed * SpeedFactor;
}

void UDodCharacterMovementComp::SetCurrentSpeedFactor(float NewSpeedFactor)
{
	SpeedFactor = NewSpeedFactor;
}
