#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DodInventoryManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOD_API UDodInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDodInventoryManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};

//!!! 当前只考虑武器和配件（游戏设置最大10个）
// 玩家一共四种装备
// 武器：
// 武器暂时不分主武器和副武器，所有武器共享两个位置
// 游戏中分四种情况：
// COD20 多人：主武器/副武器，可通过perk变为两个主武器或两个副武器
// COD20 僵尸模式：两把武器，不分种类
// COD21 战区：主武器/副武器/近战，可以通过perk变为两个主武器+近战或两个副武器+近战
// COD21 僵尸模式：开局只有一把武器，之后两把武器不分种类+近战
// 战术装备|杀伤装备
// 战场升级
// 故仓库设计：
// 战术装备->可装备物品
// 杀伤装备->可装备物品
// 战场升级->可装备物品
// 武器->可装备物品
// 仓库只需要管理可装备物品即可
// 仓库四个列表：TArray 武器，TMap<装备，数量定义（当前和最大）> 杀伤|升级|战场升级
// 武器：
// 仓库物品定义（组成：可装备物品定义，Icon，HUD，配件列表，武器基础属性（这个属性会被配件列表Additive））
// 可装备物品定义（组成：武器模型，武器插槽和偏移量）
// 战术|杀伤|战场升级：
// 仓库物品定义（组成：可装备物品定义，Icon，属性，HUD）
// 可装备物品定义（组成：模型）