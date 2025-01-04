#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "DodStaminaComponent.generated.h"

class UDodAbilitySystemComponent;
class UDodStaminaSet;
/**
 * UDodStaminaComponent
 *
 * 处理与耐力相关的组件
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DOD_API UDodStaminaComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UDodStaminaComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UDodAbilitySystemComponent> AbilitySystemComponent{nullptr};

	UPROPERTY()
	TObjectPtr<const UDodStaminaSet> StaminaSet{nullptr};
};
