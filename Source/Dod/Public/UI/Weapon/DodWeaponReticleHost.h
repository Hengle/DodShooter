#pragma once

#include "CoreMinimal.h"
#include "DodWeaponUI.h"
#include "DodWeaponReticleHost.generated.h"

class UDodHealthComponent;
class UOverlay;
class USizeBox;

UCLASS()
class DOD_API UDodWeaponReticleHost : public UDodWeaponUI
{
	GENERATED_BODY()

public:
	virtual void OnWeaponChanged_Implementation(UDodWeaponInstance* OldWeapon, UDodWeaponInstance* NewWeapon) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void OnDeathStarted(AActor* OwningActor);

protected:
	void ClearExistingWidgets();

	void OnCheckForDeath();

	void UpdateVisibility();

	UPROPERTY(meta=(BindWidget))
	USizeBox* VisWrapper;
	UPROPERTY(meta=(BindWidget))
	UOverlay* WidgetStack;

	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> SpawnedWidgets;

	UPROPERTY()
	UDodHealthComponent* NewHealthComp;
	UPROPERTY()
	UDodHealthComponent* CurrentHealthComp;
};
