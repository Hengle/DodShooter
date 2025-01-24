#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DodAttributeSet.h"
#include "DodCombatSet.generated.h"

UCLASS()
class DOD_API UDodCombatSet : public UDodAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UDodCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UDodCombatSet, BaseHeal);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "Dod|Combat",
		Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage{0.f};

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "Dod|Combat",
		Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal{0.f};
};
