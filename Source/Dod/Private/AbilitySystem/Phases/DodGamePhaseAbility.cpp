#include "AbilitySystem/Phases/DodGamePhaseAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Phases/DodGamePhaseSubsystem.h"

UDodGamePhaseAbility::UDodGamePhaseAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
}

void UDodGamePhaseAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo->IsNetAuthority())
	{
		UWorld* World = ActorInfo->AbilitySystemComponent->GetWorld();
		UDodGamePhaseSubsystem* PhaseSubsystem = UWorld::GetSubsystem<UDodGamePhaseSubsystem>(World);
		PhaseSubsystem->OnBeginPhase(this, Handle);
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDodGamePhaseAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActorInfo->IsNetAuthority())
	{
		UWorld* World = ActorInfo->AbilitySystemComponent->GetWorld();
		UDodGamePhaseSubsystem* PhaseSubsystem = UWorld::GetSubsystem<UDodGamePhaseSubsystem>(World);
		PhaseSubsystem->OnEndPhase(this, Handle);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
