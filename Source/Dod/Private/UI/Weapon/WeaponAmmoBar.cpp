#include "UI/Weapon/WeaponAmmoBar.h"

#include "Components/HorizontalBox.h"
#include "Equipment/DodQuickBarComponent.h"
#include "Inventory/DodInventoryItemInstance.h"
#include "Inventory/IIF_QuickBarIcon.h"
#include "CommonTextBlock.h"
#include "DodGameplayTags.h"
#include "Components/Image.h"

void UWeaponAmmoBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetQuickBar())
	{
		UDodInventoryItemInstance* NewInventoryItem = GetQuickBar()->GetActiveSlotItem();
		if (NewInventoryItem && NewInventoryItem != InventoryItem)
		{
			InventoryItem = NewInventoryItem;
			// 获取图标
			if (const UIIF_QuickBarIcon* QuickBarIcon = InventoryItem->FindFragmentByClass<UIIF_QuickBarIcon>())
			{
				AmmoIcon->SetBrush(QuickBarIcon->AmmoBrush);
			}
		}
	}
	else
	{
		InventoryItem = nullptr;
	}
	if (InventoryItem)
	{
		HorizontalBoxForHiding->SetVisibility(ESlateVisibility::Visible);
		int32 MagazineAmmo = InventoryItem->GetStatTagStackCount(DodGameplayTags::ShooterGame_Weapon_MagazineAmmo);
		int32 SpareAmmo = InventoryItem->GetStatTagStackCount(DodGameplayTags::ShooterGame_Weapon_SpareAmmo);
		AmmoLeftInMagazineWidget->SetText(FText::AsNumber(MagazineAmmo));
		TotalCountWidget->SetText(FText::AsNumber(SpareAmmo));
	}
	else
	{
		HorizontalBoxForHiding->SetVisibility(ESlateVisibility::Hidden);
	}
}

UDodQuickBarComponent* UWeaponAmmoBar::GetQuickBar()
{
	if (QuickBar)
	{
		return QuickBar;
	}
	QuickBar = GetOwningPlayer()->GetComponentByClass<UDodQuickBarComponent>();
	return QuickBar;
}
