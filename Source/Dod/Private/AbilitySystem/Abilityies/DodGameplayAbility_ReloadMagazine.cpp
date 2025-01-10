#include "AbilitySystem/Abilityies/DodGameplayAbility_ReloadMagazine.h"

#include "AbilitySystemComponent.h"
#include "DodGameplayTags.h"
#include "Character/DodCharacter.h"
#include "Character/DodShooter.h"
#include "Inventory/DodInventoryItemInstance.h"
#include "Weapon/DodRangedWeaponInstance.h"

void UDodGameplayAbility_ReloadMagazine::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                         const FGameplayAbilityActorInfo* ActorInfo,
                                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bDidBlockFiring = GetAssociatedItem()->GetStatTagStackCount(DodGameplayTags::Weapon_MagazineAmmo) <= 0;
	if (bDidBlockFiring)
	{
		ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(DodGameplayTags::Ability_Weapon_NoFiring);
	}
	// 播放蒙太奇
	// 只需要播放角色换弹动作的蒙太奇
	// 武器的蒙太奇在角色换弹中，有编写对应的动画通知类
	// 同理，配件换弹动画的在武器中
	ADodCharacter* Character = GetDodCharacterFromActorInfo();
	Character->GetMesh()->GetAnimInstance()->Montage_Play(BodyReloadMontage);
	if (ADodShooter* Player = Cast<ADodShooter>(Character))
	{
		Player->ArmMesh->GetAnimInstance()->Montage_Play(ArmReloadMontage);
	}
	// 从RangedWeaponInstance上获取换弹时长
	UDodRangedWeaponInstance* Weapon = Cast<UDodRangedWeaponInstance>(GetAssociatedEquipment());
	if (Weapon)
	{
		if (HasAuthority(&ActivationInfo))
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadAddTimerHandle, this, &ThisClass::ReloadAmmoIntoMagazine,
			                                       Weapon->ReloadAddTime);
		}
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}), Weapon->ReloadTime, false);
	}
}

void UDodGameplayAbility_ReloadMagazine::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                                    bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (bDidBlockFiring)
	{
		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(DodGameplayTags::Ability_Weapon_NoFiring);
	}
}

void UDodGameplayAbility_ReloadMagazine::ReloadAmmoIntoMagazine()
{
	UDodInventoryItemInstance* ItemInstance = GetAssociatedItem();

	int32 MagazineSize = ItemInstance->GetStatTagStackCount(DodGameplayTags::ShooterGame_Weapon_MagazineSize);
	int32 StartingSpareAmmo = ItemInstance->GetStatTagStackCount(DodGameplayTags::ShooterGame_Weapon_SpareAmmo);
	int32 RemainingMagazineAmmo = ItemInstance->GetStatTagStackCount(DodGameplayTags::ShooterGame_Weapon_MagazineAmmo);

	int32 TotalAmmo = StartingSpareAmmo + RemainingMagazineAmmo;
	int32 NumToAddToMagazine = FMath::Min(TotalAmmo, MagazineSize) - RemainingMagazineAmmo;

	ItemInstance->AddStatTagStack(DodGameplayTags::ShooterGame_Weapon_MagazineAmmo, NumToAddToMagazine);
	ItemInstance->RemoveStatTagStack(DodGameplayTags::ShooterGame_Weapon_SpareAmmo, NumToAddToMagazine);
}
