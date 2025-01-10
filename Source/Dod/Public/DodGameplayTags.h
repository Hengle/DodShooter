#pragma once

#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace DodGameplayTags
{
	DOD_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	DOD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Movement_ADS);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Movement_Melee);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Movement_Reload);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Movement_WeaponFire);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	DOD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	DOD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	DOD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	DOD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Weapon_NoFiring);
	
	DOD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_MagazineAmmo);

	DOD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShooterGame_Weapon_MagazineSize);
	DOD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShooterGame_Weapon_SpareAmmo);
	DOD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShooterGame_Weapon_MagazineAmmo);
} // namespace DodGameplayTags
