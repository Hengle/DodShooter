#include "UI/Weapon/Reticle_Rifle.h"

#include "DodGameplayTags.h"
#include "Animation/WidgetAnimation.h"
#include "Messages/UIMessaging.h"
#include "Player/DodPlayerState.h"


void UReticle_Rifle::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	ADSMessageListener = MessageSystem.RegisterListener(DodGameplayTags::Gameplay_Message_ADS, this,
	                                                    &ThisClass::OnReceiveADS);
	EliminationMessageListener = MessageSystem.RegisterListener(DodGameplayTags::Dod_Elimination_Message, this,
	                                                            &ThisClass::OnReceiveElimination);
}

void UReticle_Rifle::NativeDestruct()
{
	Super::NativeDestruct();

	ADSMessageListener.Unregister();
	EliminationMessageListener.Unregister();
}

void UReticle_Rifle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 调整散布
}

void UReticle_Rifle::OnReceiveADS(FGameplayTag Channel, const FUIMessaging& Payload)
{
	if (Payload.Controller == GetOwningPlayer())
	{
		ADS(Payload.bOn);
	}
}

void UReticle_Rifle::OnReceiveElimination(FGameplayTag Channel, const FDodVerbMessage& Payload)
{
	if (ADodPlayerState* PS = Cast<ADodPlayerState>(Payload.Instigator))
	{
		if (APlayerController* PC = PS->GetPlayerController())
		{
			if (Payload.Instigator != Payload.Target && PC == GetOwningPlayer())
			{
				Eliminate();
			}
		}
	}
}

void UReticle_Rifle::ADS(bool bInOn)
{
}

void UReticle_Rifle::Eliminate()
{
	if (IsAnimationPlayingForward(EliminationAnim))
	{
		StopAnimation(EliminationAnim);
		SetAnimationCurrentTime(EliminationAnim, 0.f);
	}
	PlayAnimationForward(EliminationAnim);
}
