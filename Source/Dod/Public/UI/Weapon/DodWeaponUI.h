#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DodWeaponUI.generated.h"

class UDodWeaponInstance;

UCLASS()
class DOD_API UDodWeaponUI : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnWeaponChanged(UDodWeaponInstance* OldWeapon, UDodWeaponInstance* NewWeapon);

private:
	UPROPERTY(Transient)
	TObjectPtr<UDodWeaponInstance> CurrentInstance;
};
