#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DodGameplayAbility.generated.h"

class UDodOperatorComponent;
class ADodPlayerController;
class ADodCharacter;
class UDodAbilityCost;
class IDodAbilitySourceInterface;

UENUM(BlueprintType)
enum class EDodAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnSpawn
};

UENUM(BlueprintType)
enum class EDodAbilityActivationGroup : uint8
{
	Independent,
	Exclusive_Replaceable,
	Exclusive_Blocking,
	MAX UMETA(Hidden)
};

UCLASS()
class DOD_API UDodGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class UDodAbilitySystemComponent;

public:
	UDodGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Dod|Ability")
	ADodPlayerController* GetDodPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintCallable, Category = "Dod|Ability")
	AController* GetControllerFromActorInfo();

	UFUNCTION(BlueprintCallable, Category = "Dod|Ability")
	ADodCharacter* GetDodCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Dod|Ability")
	UDodOperatorComponent* GetDodOperatorComponentFromActorInfo() const;

	EDodAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:
	//~ Begin UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags = nullptr,
	                                const FGameplayTagContainer* TargetTags = nullptr,
	                                FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                       FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                       const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle,
	                                                       const FGameplayAbilityActorInfo* ActorInfo) const override;
	virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec,
	                                                  FGameplayAbilitySpec* AbilitySpec) const override;
	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent,
	                                               const FGameplayTagContainer* SourceTags = nullptr,
	                                               const FGameplayTagContainer* TargetTags = nullptr,
	                                               FGameplayTagContainer* OptionalRelevantTags = nullptr)
	const override;
	//~ End of UGameplayAbility interface

	virtual void OnPawnAvatarSet();

	virtual void GetAbilitySource(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                              float& OutSourceLevel, const IDodAbilitySourceInterface*& OutAbilitySource,
	                              AActor*& OutEffectCauser) const;

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityRemoved")
	void K2_OnAbilityRemoved();
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();

	UPROPERTY(EditDefaultsOnly, Instanced, Category = Costs)
	TArray<TObjectPtr<UDodAbilityCost>> AdditionalCosts;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Ability Activation")
	EDodAbilityActivationPolicy ActivationPolicy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Ability Activation")
	EDodAbilityActivationGroup ActivationGroup;
};
