#include "UI/Widget/HostSessionScreen.h"

#include "CommonUserSubsystem.h"
#include "PrimaryGameLayout.h"
#include "UI/Foundation/DodLoadingScreenSubsystem.h"
#include "UI/Widget/GamePlayList.h"
#include "NativeGameplayTags.h"
#include "GameMode/DodUserFacingExperienceDefinition.h"

namespace FrontendTags
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
}

void UHostSessionScreen::NativeConstruct()
{
	Super::NativeConstruct();

	GameplayList->OnExperienceSelected.AddDynamic(this, &ThisClass::OnExperienceSelected);
}

void UHostSessionScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UHostSessionScreen::OnExperienceSelected(UDodUserFacingExperienceDefinition* ExpDef)
{
	SelectedExperience = ExpDef;
	if (SelectedExperience)
	{
		if (OnlineMode == ECommonSessionOnlineMode::Online)
		{
			if (UCommonUserSubsystem* UserSys = GetGameInstance()->GetSubsystem<UCommonUserSubsystem>())
			{
				FCommonUserInitializeParams Params;
				Params.RequestedPrivilege = ECommonUserPrivilege::CanPlayOnline;
				Params.LocalPlayerIndex = 0;
				Params.bCanCreateNewLocalPlayer = false;
				Params.OnUserInitializeComplete.BindUFunction(this, TEXT("HandleInitializationComplete"));
				UserSys->TryToInitializeUser(Params);
			}
		}
		else
		{
			LoginGame();
		}
	}
}

void UHostSessionScreen::LoginGame()
{
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this))
	{
		constexpr bool bSuspendInputUntilComplete = true;
		RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(
			FrontendTags::TAG_UI_LAYER_MENU, bSuspendInputUntilComplete, NonInteractiveWidget,
			[this](EAsyncWidgetLayerState State, UCommonActivatableWidget* InWidget)
			{
				SpinnerWidget = InWidget;
				LoginRequest();
			});
	}
}

void UHostSessionScreen::LoginRequest()
{
	if (UDodLoadingScreenSubsystem* LSS = GetGameInstance()->GetSubsystem<UDodLoadingScreenSubsystem>())
	{
		if (TSubclassOf<UUserWidget> LoadingWidget = SelectedExperience->LoadingScreenWidget.LoadSynchronous())
		{
			LSS->SetLoadingScreenContentWidget(LoadingWidget);
		}
	}
	UCommonSession_HostSessionRequest* SessionRequest = CreateHostingRequest();

	if (UCommonSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UCommonSessionSubsystem>())
	{
		SessionCreatedDelegateHandle =
			SessionSubsystem->OnCreateSessionCompleteEvent.AddUFunction(this, TEXT("OnCreateSessionComplete"));
		SessionSubsystem->HostSession(GetOwningPlayer(), SessionRequest);
	}
}

UCommonSession_HostSessionRequest* UHostSessionScreen::CreateHostingRequest()
{
	if (SelectedExperience)
	{
		UCommonSession_HostSessionRequest* Request = SelectedExperience->CreateHostingRequest(GetWorld());
		return Request;
	}
	return nullptr;
}

void UHostSessionScreen::OnCreateSessionComplete(const FOnlineResultInformation& Result)
{
	if (UCommonSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UCommonSessionSubsystem>())
	{
		SessionSubsystem->OnCreateSessionCompleteEvent.Remove(SessionCreatedDelegateHandle);
	}
	if (!Result.bWasSuccessful)
	{
		UCommonUIExtensions::PopContentFromLayer(SpinnerWidget);
	}
}

void UHostSessionScreen::HandleInitializationComplete(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error,
                                                      ECommonUserPrivilege RequestedPrivilege,
                                                      ECommonUserOnlineContext OnlineContext)
{
	LoginGame();
}
