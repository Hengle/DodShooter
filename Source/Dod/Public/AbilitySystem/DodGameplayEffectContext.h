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
	FDodGameplayEffectContext()
	: FGameplayEffectContext()
	{
	}

	FDodGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}
	
	static DOD_API FDodGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	void SetAbilitySource(const IDodAbilitySourceInterface* InObject, float InSourceLevel);

	virtual FGameplayEffectContext* Duplicate() const override
	{
		FDodGameplayEffectContext* NewContext = new FDodGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};

template<>
struct TStructOpsTypeTraits<FDodGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FDodGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

