#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DodAttributeSet.h"
#include "DodHealthSet.generated.h"

/**
 * UDodHealthSet
 * 
 * 生命值和护甲
 */
UCLASS()
class DOD_API UDodHealthSet : public UDodAttributeSet
{
	GENERATED_BODY()

public:
	mutable FDodAttributeEvent OnHealthChanged;
	mutable FDodAttributeEvent OnMaxHealthChanged;
	mutable FDodAttributeEvent OnOutOfHealth;

	ATTRIBUTE_ACCESSORS(UDodHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UDodHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UDodHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UDodHealthSet, Damage);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//~ Begin AttributeSet interface
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//~ End of AttributeSet interface

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	// 最大值修改时，当前值按比例改变
	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
	                                 const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
	                                 const FGameplayAttribute& AffectedAttributeProperty)const;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Dod|Health",
		Meta = (HideFromModifiers))
	FGameplayAttributeData Health{150.f};

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Dod|Health",
		Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth{150.f};

	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;

	// -------------------------------------------------------------------
	//	Meta Attribute
	// -------------------------------------------------------------------

	// +Health
	UPROPERTY(BlueprintReadOnly, Category="Dod|Health")
	FGameplayAttributeData Healing;

	// -Health
	UPROPERTY(BlueprintReadOnly, Category="Dod|Health", Meta=(HideFromModifiers))
	FGameplayAttributeData Damage;

	bool bOutOfHealth{false};
};
