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

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	//~ Begin UAbilitySystemComponent interface
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
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
