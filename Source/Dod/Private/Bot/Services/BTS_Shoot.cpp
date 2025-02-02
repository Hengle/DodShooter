#include "Bot/Services/BTS_Shoot.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "DodGameplayTags.h"
#include "GameplayPrediction.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "Equipment/DodEquipmentManagerComponent.h"
#include "Inventory/DodInventoryItemInstance.h"
#include "Weapon/DodWeaponInstance.h"

void UBTS_Shoot::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (AIOwner)
	{
		if (CurrentWeaponCanShoot(AIOwner->GetPawn(), AIOwner))
		{
			UAbilitySystemComponent* AbilitySystemComponent =
				UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AIOwner->GetPawn());
			if (AbilitySystemComponent != nullptr && IsValidChecked(AbilitySystemComponent))
			{
				FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
				FGameplayEventData Payload;
				AbilitySystemComponent->HandleGameplayEvent(
					FGameplayTag::RequestGameplayTag(TEXT("InputTag.Weapon.Fire")),
					&Payload);
			}
		}
		else
		{
			UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
			Blackboard->SetValue<UBlackboardKeyType_Bool>(OutOfAmmo.SelectedKeyName, true);
		}
	}
}

bool UBTS_Shoot::CurrentWeaponCanShoot(APawn* InPawn, AController* InController)
{
	if (!InPawn)
	{
		return false;
	}
	UDodEquipmentManagerComponent* EMC = InPawn->GetComponentByClass<UDodEquipmentManagerComponent>();
	if (!EMC)
	{
		return false;
	}
	UDodEquipmentInstance* Item = EMC->GetFirstInstanceOfType(UDodWeaponInstance::StaticClass());
	if (!Item)
	{
		return false;
	}
	UDodInventoryItemInstance* CurrentWeaponDef = Cast<UDodInventoryItemInstance>(Item->GetInstigator());
	if (!CurrentWeaponDef)
	{
		return false;
	}
	int32 SpareAmmo = CurrentWeaponDef->GetStatTagStackCount(DodGameplayTags::ShooterGame_Weapon_SpareAmmo);
	int32 MagAmmo = CurrentWeaponDef->GetStatTagStackCount(DodGameplayTags::ShooterGame_Weapon_MagazineAmmo);
	if (SpareAmmo + MagAmmo > 0)
	{
		return true;
	}
	if (SpareAmmo >= 0 && MagAmmo <= 0)
	{
		return false;
	}
	// TODO 切换到有子弹的武器
	return false;
}
