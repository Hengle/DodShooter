#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "Abilityies/DodGameplayAbility.h"
#include "DodAbilitySystemComponent.generated.h"

DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityInputBlocked);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOD_API UDodAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UDodAbilitySystemComponent();

	typedef TFunctionRef<bool(const UDodGameplayAbility* DodAbility, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();


	void AddAbilityToActivationGroup(EDodAbilityActivationGroup Group, UDodGameplayAbility* DodAbility);
	void RemoveAbilityFromActivationGroup(EDodAbilityActivationGroup Group, UDodGameplayAbility* DodAbility);
	void CancelActivationGroupAbilities(EDodAbilityActivationGroup Group, UDodGameplayAbility* IgnoreDodAbility, bool bReplicateCancelAbility);

	//~ Begin UAbilitySystemComponent interface
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	//~ End of UAbilitySystemComponent interface

	bool IsActivationGroupBlocked(EDodAbilityActivationGroup Group) const;

	void GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle,
	                          FGameplayAbilityActivationInfo ActivationInfo,
	                          FGameplayAbilityTargetDataHandle& OutTargetDataHandle);

	void GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags,
	                                            FGameplayTagContainer& OutActivationRequired,
	                                            FGameplayTagContainer& OutActivationBlocked) const;

protected:
	void TryActivateAbilitiesOnSpawn();

	// UPROPERTY()
	// TObjectPtr<UDodAbilityTagRelationshipMapping> TagRelationshipMapping;

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	int32 ActivationGroupCounts[(uint8)EDodAbilityActivationGroup::MAX];
};
