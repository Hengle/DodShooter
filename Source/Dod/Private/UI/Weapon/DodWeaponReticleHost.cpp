#include "UI/Weapon/DodWeaponReticleHost.h"

#include "Character/Comp/DodHealthComponent.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Inventory/DodInventoryItemInstance.h"
#include "Inventory/IIF_ReticleConfig.h"
#include "Weapon/DodWeaponInstance.h"
#include "UI/Weapon/DodReticleWidgetBase.h"

void UDodWeaponReticleHost::OnWeaponChanged_Implementation(UDodWeaponInstance* OldWeapon, UDodWeaponInstance* NewWeapon)
{
	Super::OnWeaponChanged_Implementation(OldWeapon, NewWeapon);

	ClearExistingWidgets();
	if (NewWeapon)
	{
		if (UDodInventoryItemInstance* ItemInstance = Cast<UDodInventoryItemInstance>(NewWeapon->GetInstigator()))
		{
			if (const UIIF_ReticleConfig* ReticleConfig = ItemInstance->FindFragmentByClass<UIIF_ReticleConfig>())
			{
				for (TSubclassOf<UDodReticleWidgetBase> ReticleWdiget : ReticleConfig->ReticleWidgets)
				{
					UDodReticleWidgetBase* CreatedReticle = CreateWidget<UDodReticleWidgetBase>(
						GetOwningPlayer(), ReticleWdiget);
					if (CreatedReticle)
					{
						CreatedReticle->InitializeFromWeapon(NewWeapon);
						SpawnedWidgets.Add(CreatedReticle);
						UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(WidgetStack->AddChild(CreatedReticle));
						OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
						OverlaySlot->SetVerticalAlignment(VAlign_Fill);
					}
				}
			}
		}
	}
}

void UDodWeaponReticleHost::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	OnCheckForDeath();
}

void UDodWeaponReticleHost::OnDeathStarted(AActor* OwningActor)
{
	UpdateVisibility();
	CurrentHealthComp->OnDeathStarted.RemoveDynamic(this, &ThisClass::OnDeathStarted);
}

void UDodWeaponReticleHost::ClearExistingWidgets()
{
	for (TObjectPtr<UUserWidget> Widget : SpawnedWidgets)
	{
		WidgetStack->RemoveChild(Widget);
	}
	SpawnedWidgets.Empty();
}

void UDodWeaponReticleHost::OnCheckForDeath()
{
	if (!GetOwningPlayer() || !GetOwningPlayer()->GetPawn())
	{
		return;
	}
	if (UDodHealthComponent* HealthComp = GetOwningPlayer()->GetPawn()->GetComponentByClass<UDodHealthComponent>())
	{
		NewHealthComp = HealthComp;
	}
	else
	{
		NewHealthComp = nullptr;
	}
	if (NewHealthComp != CurrentHealthComp)
	{
		CurrentHealthComp = NewHealthComp;
		if (CurrentHealthComp)
		{
			CurrentHealthComp->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
		}
		UpdateVisibility();
	}
}

void UDodWeaponReticleHost::UpdateVisibility()
{
	if (CurrentHealthComp)
	{
		if (CurrentHealthComp->IsDeadOrDying())
		{
			VisWrapper->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			VisWrapper->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
