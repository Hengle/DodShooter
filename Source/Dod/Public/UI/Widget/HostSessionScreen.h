#pragma once

#include "CoreMinimal.h"
#include "CommonSessionSubsystem.h"
#include "UI/DodActivatableWidget.h"
#include "HostSessionScreen.generated.h"

class UCommonUserInfo;

UCLASS()
class DOD_API UHostSessionScreen : public UDodActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnActivated() override;

protected:
	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_LoginGame();
	virtual void LoginGame();

	UCommonSession_HostSessionRequest* CreateHostingRequest();

private:
	UFUNCTION()
	virtual void HandleInitializationComplete(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error,
	                                          ECommonUserPrivilege RequestedPrivilege,
	                                          ECommonUserOnlineContext OnlineContext);

	ECommonSessionOnlineMode OnlineMode{ECommonSessionOnlineMode::Online};
};
