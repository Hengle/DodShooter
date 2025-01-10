#pragma once

#include "CoreMinimal.h"
#include "UI/DodTaggedWidget.h"
#include "WeaponAmmoBar.generated.h"

class UImage;
class UCommonTextBlock;
class UHorizontalBox;
class UDodInventoryItemInstance;
class UDodQuickBarComponent;

UCLASS()
class DOD_API UWeaponAmmoBar : public UDodTaggedWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UDodQuickBarComponent* GetQuickBar();

	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* HorizontalBoxForHiding;
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* AmmoLeftInMagazineWidget;
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* TotalCountWidget;
	UPROPERTY(meta=(BindWidget))
	UImage* AmmoIcon;

private:
	UPROPERTY(Transient)
	UDodQuickBarComponent* QuickBar;
	UPROPERTY(Transient)
	UDodInventoryItemInstance* InventoryItem;
};
