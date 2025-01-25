#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "DodAbilitySystemGlobals.generated.h"

UCLASS(Config=Game)
class DOD_API UDodAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	UDodAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	//~UAbilitySystemGlobals interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	//~End of UAbilitySystemGlobals interface
};
