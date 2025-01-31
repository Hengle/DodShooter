#include "DodGameplayTags.h"

#include "GameplayTagsManager.h"

namespace DodGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_IsDead, "Ability.ActivateFail.IsDead",
	                               "Ability failed to activate because its owner is dead.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_ActivationGroup, "Ability.ActivateFail.ActivationGroup",
	                               "Ability failed to activate because of its activation group.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Behavior_SurvivesDeath, "Ability.Behavior.SurvivesDeath",
								   "An ability with this type tag should not be canceled due to death.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Respawn_Completed_Message, "Ability.Respawn.Completed.Message", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Respawn_Duration_Message, "Ability.Respawn.Duration.Message", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_StatusChange_Death, "Ability.Type.StatusChange.Death", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (stick) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_AutoRun, "InputTag.AutoRun", "Auto-run input.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Movement_ADS, "Event.Movement.ADS", "Weapon ADS");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Movement_Melee, "Event.Movement.Melee", "Weapon Melee");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Movement_Reload, "Event.Movement.Reload", "Weapon Reload");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Movement_WeaponFire, "Event.Movement.WeaponFire", "Weapon WeaponFire");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "InitState.Spawned", "1: Actor/组件已初步生成，可以扩展");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable, "InitState.DataAvailable", "2: 所有必需的数据已加载/复制，准备初始化");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized, "InitState.DataInitialized",
	                               "3: 该 Actor/组件的可用数据已初始化，但尚未准备好完全进入gameplay");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "4: Actor/组件已完全准备好进行激活gameplay");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death. This event only fires on the server.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Reset, "GameplayEvent.Reset", "Event that fires once a player reset is executed.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_RequestReset, "GameplayEvent.RequestReset", "Event to request a player's pawn to be instantly replaced with a new one at a valid spawn location.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Damage, "SetByCaller.Damage", "SetByCaller tag used by damage gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Heal, "SetByCaller.Heal", "SetByCaller tag used by healing gameplay effects.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Crouching, "Status.Crouching", "Target is crouching.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Target has the death status.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dying, "Status.Death.Dying", "Target has begun the death process.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dead, "Status.Death.Dead", "Target has finished the death process.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Weapon_NoFiring, "Ability.Weapon.NoFiring", "Ability_Weapon_NoFiring.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_MagazineAmmo, "Weapon.MagazineAmmo", "Weapon Magazine Ammo.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShooterGame_Weapon_MagazineSize, "ShooterGame.Weapon.MagazineSize", "弹匣可容纳的子弹数量");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShooterGame_Weapon_SpareAmmo, "ShooterGame.Weapon.SpareAmmo", "玩家剩余子弹数量");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShooterGame_Weapon_MagazineAmmo, "ShooterGame.Weapon.MagazineAmmo", "弹匣中剩余子弹数量");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShooterGame_Score_Eliminations, "ShooterGame.Score.Eliminations", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShooterGame_Score_Assists, "ShooterGame.Score.Assists", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShooterGame_Score_Deaths, "ShooterGame.Score.Deaths", "");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Dod_Elimination_Message, "Dod.Elimination.Message", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Dod_Assist_Message, "Dod.Assist.Message", "");

	FGameplayTag DodGameplayTags::FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				if (TestTag.ToString().Contains(TagString))
				{
					Tag = TestTag;
					break;
				}
			}
		}

		return Tag;
	}
} // namespace DodGameplayTags
