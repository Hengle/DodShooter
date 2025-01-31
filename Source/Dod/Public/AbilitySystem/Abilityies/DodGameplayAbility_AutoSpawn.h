#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbility.h"
#include "UIExtensionSubsystem.h"
#include "DodGameplayAbility_AutoSpawn.generated.h"

class UDodHealthComponent;
struct FDodPlayerResetMessage;

UCLASS()
class DOD_API UDodGameplayAbility_AutoSpawn : public UDodGameplayAbility
{
	GENERATED_BODY()

public:
	UDodGameplayAbility_AutoSpawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnAbilityAdded() override;
	virtual void OnAbilityRemoved() override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	virtual void OnPawnAvatarSet() override;

protected:
	void ReadyForSpawn();

	void BindDeathListener();
	void ClearDeathListener();

	bool IsAvatarDeadOrDying() const;

	UFUNCTION()
	void OnReceiveReset(FGameplayTag Channel, const FDodPlayerResetMessage& Payload);

	UFUNCTION()
	void OnAvatarEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	void AvatarEndPlay();
	void ContinueEndPlay();
	UFUNCTION()
	void SpawnPlayer();

	AActor* GetOwningPlayerState();

	void EndDeathAbilities(UAbilitySystemComponent* ASC);

	UFUNCTION()
	void OnDeathStarted(AActor* OwningActor);

private:
	UPROPERTY()
	AActor* LastBoundAvatarActor{nullptr};
	UPROPERTY()
	UDodHealthComponent* LastBoundHealthComponent{nullptr};

	UPROPERTY()
	AController* ControllerToReset;

protected:
	float RespawnDelayDuration{3.f};

private:
	FTimerHandle DeathTimerHandle;

	FUIExtensionHandle Extension;
	
	bool bIsListeningForReset{false};
	bool bShouldFinishRestart{false};

};
