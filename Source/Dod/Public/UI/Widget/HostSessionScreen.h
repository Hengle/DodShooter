#pragma once

#include "CoreMinimal.h"
#include "CommonSessionSubsystem.h"
#include "UI/DodActivatableWidget.h"
#include "HostSessionScreen.generated.h"

class UDodUserFacingExperienceDefinition;
class UGamePlayList;
class UCommonUserInfo;

UCLASS()
class DOD_API UHostSessionScreen : public UDodActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;

protected:
	UFUNCTION()
	void OnExperienceSelected(UDodUserFacingExperienceDefinition* ExpDef);
	
	virtual void LoginGame();
	void LoginRequest();

	UCommonSession_HostSessionRequest* CreateHostingRequest();

	UFUNCTION()
	void OnCreateSessionComplete(const FOnlineResultInformation& Result);

private:
	UFUNCTION()
	virtual void HandleInitializationComplete(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error,
	                                          ECommonUserPrivilege RequestedPrivilege,
	                                          ECommonUserOnlineContext OnlineContext);

protected:
	UPROPERTY(meta=(BindWidget))
	UGamePlayList* GameplayList;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UDodActivatableWidget> NonInteractiveWidget;

private:
	UPROPERTY()
	UDodUserFacingExperienceDefinition* SelectedExperience;

	UPROPERTY()
	UCommonActivatableWidget* SpinnerWidget;

	FDelegateHandle SessionCreatedDelegateHandle;
	
	ECommonSessionOnlineMode OnlineMode{ECommonSessionOnlineMode::Online};
};
