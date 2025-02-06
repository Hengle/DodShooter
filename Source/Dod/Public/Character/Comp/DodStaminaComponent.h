#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/DodStaminaSet.h"
#include "Components/GameFrameworkComponent.h"
#include "DodStaminaComponent.generated.h"

class UDodAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDodStamina_AttributeChanged, UDodStaminaComponent*, StaminaComponent,
											  float, OldValue, float, NewValue, AActor*, Instigator);

/**
 * UDodStaminaComponent
 *
 * 处理与耐力相关的组件
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DOD_API UDodStaminaComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UDodStaminaComponent(const FObjectInitializer& ObjectInitializer);
	virtual void OnUnregister() override;

	UFUNCTION(BlueprintPure, Category = "Dod|Stamina")
	static UDodStaminaComponent* FindStaminaComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<UDodStaminaComponent>() : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Dod|Stamina")
	void InitializeWithAbilitySystem(UDodAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category = "Dod|Stamina")
	void UninitializeFromAbilitySystem();

	UFUNCTION(BlueprintCallable, Category = "Dod|Stamina")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Dod|Stamina")
	float GetMaxStamina() const;

protected:
	virtual void BeginPlay() override;

	virtual void HandleStaminaChanged(AActor* DamageInstigator, AActor* DamageCauser,
								 const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue,
								 float NewValue);
	virtual void HandleMaxStaminaChanged(AActor* DamageInstigator, AActor* DamageCauser,
										const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude,
										float OldValue, float NewValue);

public:
	UPROPERTY(BlueprintAssignable)
	FDodStamina_AttributeChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FDodStamina_AttributeChanged OnMaxStaminaChanged;

protected:
	UPROPERTY()
	TObjectPtr<UDodAbilitySystemComponent> AbilitySystemComponent{nullptr};

	UPROPERTY()
	TObjectPtr<const UDodStaminaSet> StaminaSet{nullptr};
};
