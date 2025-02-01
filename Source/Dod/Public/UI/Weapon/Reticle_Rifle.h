#pragma once

#include "CoreMinimal.h"
#include "DodReticleWidgetBase.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messages/DodVerbMessage.h"
#include "Reticle_Rifle.generated.h"

struct FUIMessaging;
struct FGameplayTag;

UCLASS()
class DOD_API UReticle_Rifle : public UDodReticleWidgetBase
{
	GENERATED_BODY()

public:
	//~ Begin UserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End UserWidget interface

	void OnReceiveADS(FGameplayTag Channel, const FUIMessaging& Payload);
	void OnReceiveElimination(FGameplayTag Channel, const FDodVerbMessage& Payload);

	void ADS(bool bInOn);
	void Eliminate();

protected:
	FGameplayMessageListenerHandle ADSMessageListener;
	FGameplayMessageListenerHandle EliminationMessageListener;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* EliminationAnim;
};
