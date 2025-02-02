#include "Bot/Services/BTS_ReloadWeapon.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "AbilitySystem/Abilityies/DodGameplayAbility_Death.h"

void UBTS_ReloadWeapon::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	UAbilitySystemComponent* AbilitySystemComponent =
		UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIOwner->GetPawn());
	if (AbilitySystemComponent != nullptr && IsValidChecked(AbilitySystemComponent))
	{
		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
		FGameplayEventData Payload;
		AbilitySystemComponent->HandleGameplayEvent(
			FGameplayTag::RequestGameplayTag(TEXT("InputTag.Weapon.Reload")), &Payload);
	}
}
