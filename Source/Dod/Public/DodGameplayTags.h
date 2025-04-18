﻿#pragma once

#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace DodGameplayTags
{
	DOD_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Respawn_Completed_Message);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Respawn_Duration_Message);
	
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_StatusChange_Death);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Movement_ADS);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Movement_Melee);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Movement_Reload);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Movement_WeaponFire);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Gait_Walk);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Gait_Sprint);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Gait_SuperSprint);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Slide);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Weapon_NoFiring);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_MagazineAmmo);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShooterGame_Weapon_MagazineSize);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShooterGame_Weapon_SpareAmmo);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShooterGame_Weapon_MagazineAmmo);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShooterGame_Score_Eliminations);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShooterGame_Score_Assists);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShooterGame_Score_Deaths);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dod_Elimination_Message);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dod_Assist_Message);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dod_AddNotification_KillFeed);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_Message_ADS);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_Zone_WeakSpot);

	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Layer_Game);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Layer_GameMenu);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Layer_Menu);
	DOD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Layer_Modal);
} // namespace DodGameplayTags
