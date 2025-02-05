#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/DodStaminaSet.h"
#include "Components/GameFrameworkComponent.h"
#include "DodStaminaComponent.generated.h"

class UDodAbilitySystemComponent;
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
	virtual void OnUnregister() override;

	UFUNCTION(BlueprintCallable, Category = "Dod|Stamina")
	void InitializeWithAbilitySystem(UDodAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category = "Dod|Stamina")
	void UninitializeFromAbilitySystem();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UDodAbilitySystemComponent> AbilitySystemComponent{nullptr};

	UPROPERTY()
	TObjectPtr<const UDodStaminaSet> StaminaSet{nullptr};
};
