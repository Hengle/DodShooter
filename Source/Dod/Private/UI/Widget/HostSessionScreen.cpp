#include "UI/Widget/HostSessionScreen.h"

#include "CommonUserSubsystem.h"
#include "UI/Foundation/DodLoadingScreenSubsystem.h"

void UHostSessionScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UHostSessionScreen::StartGame()
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

void UHostSessionScreen::LoginGame()
{
	if (UDodLoadingScreenSubsystem* LSS = GetGameInstance()->GetSubsystem<UDodLoadingScreenSubsystem>())
	{
		// TODO 设置加载界面
		// LSS->SetLoadingScreenContentWidget();
	}
	CreateHostingRequest();
}

UCommonSession_HostSessionRequest* UHostSessionScreen::CreateHostingRequest()
{
	return nullptr;
}

void UHostSessionScreen::HandleInitializationComplete(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error,
                                                      ECommonUserPrivilege RequestedPrivilege,
                                                      ECommonUserOnlineContext OnlineContext)
{
	K2_LoginGame();
	LoginGame();
}
