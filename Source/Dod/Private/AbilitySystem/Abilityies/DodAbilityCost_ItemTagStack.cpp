#include "DodAbilityCost_ItemTagStack.h"

#include "NativeGameplayTags.h"
#include "AbilitySystem/Abilityies/DodGameplayAbility.h"
#include "Inventory/DodInventoryItemInstance.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_FAIL_COST, "Ability.ActivateFail.Cost");

UDodAbilityCost_ItemTagStack::UDodAbilityCost_ItemTagStack()
{
	FailureTag = TAG_ABILITY_FAIL_COST;
}

bool UDodAbilityCost_ItemTagStack::CheckCost(const UDodGameplayAbility* Ability,
                                             const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             FGameplayTagContainer* OptionalRelevantTags) const
{
	if (const UDodGameplayAbility* EquipmentAbility =
		Cast<const UDodGameplayAbility>(Ability))
	{
		if (UDodInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);
			const bool bCanApplyCost = ItemInstance->GetStatTagStackCount(Tag) >= NumStacks;

			if (!bCanApplyCost && OptionalRelevantTags && FailureTag.IsValid())
			{
				OptionalRelevantTags->AddTag(FailureTag);
			}
			return bCanApplyCost;
		}
	}
	return false;
}

void UDodAbilityCost_ItemTagStack::ApplyCost(const UDodGameplayAbility* Ability,
                                             const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (const UDodGameplayAbility* EquipmentAbility = Cast<const UDodGameplayAbility>(Ability))
		{
			if (UDodInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

				ItemInstance->RemoveStatTagStack(Tag, NumStacks);
			}
		}
	}
}
