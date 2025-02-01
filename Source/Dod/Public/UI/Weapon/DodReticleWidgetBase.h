#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DodReticleWidgetBase.generated.h"

class UDodInventoryItemInstance;
class UDodWeaponInstance;

UCLASS()
class DOD_API UDodReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(UDodWeaponInstance* InWeapon);

	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponInitialized();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDodWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDodInventoryItemInstance> InventoryInstance;
};
