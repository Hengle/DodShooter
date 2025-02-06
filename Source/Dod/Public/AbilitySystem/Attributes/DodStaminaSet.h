#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DodAttributeSet.h"
#include "DodStaminaSet.generated.h"

/**
 * UDodStaminaSet
 * 
 * 耐力
 */
UCLASS()
class DOD_API UDodStaminaSet : public UDodAttributeSet
{
	GENERATED_BODY()

public:
	mutable FDodAttributeEvent OnStaminaChanged;
	mutable FDodAttributeEvent OnMaxStaminaChanged;

	ATTRIBUTE_ACCESSORS(UDodStaminaSet, Stamina);
	ATTRIBUTE_ACCESSORS(UDodStaminaSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UDodStaminaSet, StaminaRegenRate);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//~ Begin AttributeSet interface
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//~ End of AttributeSet interface

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Dod|Stamina")
	FGameplayAttributeData Stamina{100.f};

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Dod|Stamina")
	FGameplayAttributeData MaxStamina{100.f};

	UFUNCTION()
	void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldValue);
	// 每秒恢复的体力
	UPROPERTY(BlueprintReadOnly, Category = "Dod|Stamina", ReplicatedUsing = OnRep_StaminaRegenRate)
	FGameplayAttributeData StaminaRegenRate;

private:
	float StaminaBeforeAttributeChange;
};
