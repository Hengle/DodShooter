#pragma once

#include "CoreMinimal.h"
#include "DodWeaponInstance.h"
#include "DodRangedWeaponInstance.generated.h"

UCLASS()
class DOD_API UDodRangedWeaponInstance : public UDodWeaponInstance
{
	GENERATED_BODY()

public:
	/* 射速(rpm)，每分钟最大连续发射子弹数 */
	float FireRate{0.f};
	/* 子弹初速，后续速度由重力加速度和子弹质量等因素决定，子弹速度会不断衰减 */
	/* 目前未知子弹后续速度计算方法，使用UE默认物理算法 */
	float MuzzleVelocity{0.f};
	/* 每次射击子弹数，大于1的值一般用于霰弹枪 */
	int32 PelletCount{1};
	/* 装弹时间(s)，之后可射击 */
	float ReloadTime{0.f};
	/* 装弹时间(s)，子弹添加时间，之后可以通过切换武器或道具再切换回来射击 */
	float ReloadAddTime{0.f};
	/* 空弹匣装弹动画时间(s)，之后可射击 */
	float EmptyReloadTime{0.f};
	/* 空弹匣装弹时间(s)，之后可以通过切换武器或道具再切换回来射击 */
	float EmptyReloadAddTime{0.f};
	/* 瞄准时间(s) */
	float ADSTime{0.f};
	/* 退出瞄准时间(s) */
	float TimeToExitADS{0.f};
	/* 视野 */
	float FieldOfView{90.f};
	/* 切换武器，换出时间(s) */
	float DropTime{0.f};
	/* 切换武器，换入时间(s) */
	float RaiseTime{0.f};
	/* 奔跑到射击时间(s) */
	float SprintToFire{0.f};
	/* 战术冲刺到射击时间(s) */
	float TacticalSprintToFire{0.f};
	/* 近战伤害 */
	float MeleeDamage{0.f};
	/* 近战强制击杀次数 */
	float MeleeHitsToKill{0.f};
	/* 近战吸附距离(cm) */
	float MeleeLungeDistance{0.f};
	/* 奔跑时间(s)，基于100的耐力值，每一定时间消耗的进行转换为该武器奔跑或冲刺消耗的耐力 */
	float SprintDuration{0.f};
	/* 回中速度,回中速度越快，则武器后坐力表现越小，在每次开第二枪时能保证准星位置与第一枪保持一致 */
	float ViewCenterSpeed{0.f};
	/* 回中速度 */
	float GunCenterSpeed{0.f};
	/* 屏幕跳动垂直系数 */
	float ViewKickVerMul{0.f};
	/* 屏幕跳动水平系数 */
	float ViewKickHorMul{0.f};
	/* 枪模跳动垂直系数 */
	float GunKickVerMul{0.f};
	/* 枪模跳动水平系数 */
	float GunKickHorMul{0.f};
	/* 瞄准时最小子弹散布，角度 */
	float ADSSpreadMin{0.f};
	/* 瞄准时最大子弹散布，角度 */
	float ADSSpreadMax{0.f};
	/* 最小腰射散布，角度 */
	float MinHipFireSpread{0.f};
	/* 移动时腰射散布，角度 */
	float MovingHipFireSpread{0.f};
	/* 最大腰射散布，角度 */
	float MaxHipFireSpread{0.f};
	/* 后坐力反应 */
	float FlinchMultiplier{0.f};
	/* 后坐力反应，被击中时，视角将会受到多少影响的倍数 */
	float TargetFlinchMultiplier{0.f};
};
