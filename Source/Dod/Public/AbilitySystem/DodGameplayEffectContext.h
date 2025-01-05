#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/DodAbilitySourceInterface.h"
#include "UObject/Object.h"
#include "DodGameplayEffectContext.generated.h"

USTRUCT()
struct FDodGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	static DOD_API FDodGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	void SetAbilitySource(const IDodAbilitySourceInterface* InObject, float InSourceLevel);

protected:
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};
