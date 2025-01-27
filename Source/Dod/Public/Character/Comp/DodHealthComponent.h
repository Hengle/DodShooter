#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"

#include "DodHealthComponent.generated.h"

struct FGameplayEffectSpec;
class UDodAbilitySystemComponent;
class UDodHealthSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDodHealth_DeathEvent, AActor*, OwningActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDodHealth_AttributeChanged, UDodHealthComponent*, HealthComponent,
                                              float, OldValue, float, NewValue, AActor*, Instigator);

UENUM(BlueprintType)
enum class EDodDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

/**
 * UDodHealthComponent
 *
 * 处理与生命值和护甲相关的组件
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class DOD_API UDodHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UDodHealthComponent(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = "Dod|Health")
	static UDodHealthComponent* FindHealthComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<UDodHealthComponent>() : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Dod|Health")
	void InitializeWithAbilitySystem(UDodAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category = "Dod|Health")
	void UninitializeFromAbilitySystem();

	UFUNCTION(BlueprintCallable, Category = "Dod|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Dod|Health")
	float GetMaxHealth() const;

	// 返回 Health / MaxHealth 在 [0.0, 1.0] 之间.
	UFUNCTION(BlueprintCallable, Category = "Dod|Health")
	float GetHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "Dod|Health")
	EDodDeathState GetDeathState() const { return DeathState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Dod|Health",
		Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return DeathState > EDodDeathState::NotDead; }

	virtual void StartDeath();
	virtual void FinishDeath();

	virtual void DamageSelfDestruct(bool bFellOutOfWorld = false);

protected:
	virtual void OnUnregister() override;

	void ClearGameplayTags();

	virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser,
	                                 const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue,
	                                 float NewValue);
	virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser,
	                                    const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude,
	                                    float OldValue, float NewValue);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser,
	                               const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue,
	                               float NewValue);

protected:
	UFUNCTION()
	virtual void OnRep_DeathState(EDodDeathState OldDeathState);

public:
	UPROPERTY(BlueprintAssignable)
	FDodHealth_AttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FDodHealth_AttributeChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FDodHealth_DeathEvent OnDeathStarted;

	UPROPERTY(BlueprintAssignable)
	FDodHealth_DeathEvent OnDeathFinished;

protected:
	UPROPERTY()
	TObjectPtr<UDodAbilitySystemComponent> AbilitySystemComponent{nullptr};

	UPROPERTY()
	TObjectPtr<const UDodHealthSet> HealthSet{nullptr};

	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	EDodDeathState DeathState { EDodDeathState::NotDead };
};
