#include "GameUIPolicy.h"

#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"
#include "Blueprint/UserWidget.h"

UPrimaryGameLayout* UGameUIPolicy::GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const
{
	const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
	return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}

void UGameUIPolicy::AddLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);
}

void UGameUIPolicy::RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		Layout->RemoveFromParent();
	}
}

void UGameUIPolicy::CreateLayoutWidget(ULocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = LayoutClass.LoadSynchronous();
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UPrimaryGameLayout* NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);
			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

void UGameUIPolicy::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(
		this, [this](UCommonLocalPlayer* InLocalPlayer, APlayerController* PlayerController)
		{
			NotifyPlayerRemoved(InLocalPlayer);

			if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(InLocalPlayer))
			{
				AddLayoutToViewport(InLocalPlayer, LayoutInfo->RootLayout);
				LayoutInfo->bAddedToViewport = true;
			}
			else
			{
				CreateLayoutWidget(InLocalPlayer);
			}
		});
}

void UGameUIPolicy::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
}

void UGameUIPolicy::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
}
