#include "AbilitySystem/DodGameplayEffectContext.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationState/PropertyNetSerializerInfoRegistry.h"
#include "Serialization/GameplayEffectContextNetSerializer.h"
#endif

FDodGameplayEffectContext* FDodGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(
		FDodGameplayEffectContext::StaticStruct()))
	{
		return (FDodGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

void FDodGameplayEffectContext::SetAbilitySource(const IDodAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
}

const IDodAbilitySourceInterface* FDodGameplayEffectContext::GetAbilitySource() const
{
	return Cast<IDodAbilitySourceInterface>(AbilitySourceObject.Get());
}

bool FDodGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	return true;
}

const UPhysicalMaterial* FDodGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}


#if UE_WITH_IRIS
namespace UE::Net
{
	// Forward to FGameplayEffectContextNetSerializer
	// Note: If FDodGameplayEffectContext::NetSerialize() is modified, a custom NetSerializesr must be implemented as the current fallback will no longer be sufficient.
	UE_NET_IMPLEMENT_FORWARDING_NETSERIALIZER_AND_REGISTRY_DELEGATES(DodGameplayEffectContext,
	                                                                 FGameplayEffectContextNetSerializer);
}
#endif
