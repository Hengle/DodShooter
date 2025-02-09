#include "UI/Widget/SessionBrowserScreen.h"

#include "CommonSessionSubsystem.h"
#include "CommonUserSubsystem.h"
#include "Components/Border.h"
#include "UI/Common/DodListView.h"

void USessionBrowserScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	StartSearch();
}

void USessionBrowserScreen::StartSearch()
{
	if (bSearchLock)
	{
		return;
	}
	bSearchLock = true;
	ListView->ClearListItems();
	NoGamesBorder->SetVisibility(ESlateVisibility::Collapsed);
	if (UCommonSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UCommonSessionSubsystem>())
	{
		SearchRequest = SessionSubsystem->CreateOnlineSearchSessionRequest();
		SearchRequest->OnSearchFinished.AddUFunction(this, FName("OnSearchFinished"));
		SessionSubsystem->FindSessions(GetOwningPlayer(), SearchRequest);
	}
}

void USessionBrowserScreen::UnlockAfterSearchFinish()
{
	bSearchLock = false;
}

void USessionBrowserScreen::OnSearchFinished(bool bSucceeded, const FText& ErrorMessage)
{
	if (!bSucceeded)
	{
		if (UCommonUserSubsystem* UserSubsystem = GetGameInstance()->GetSubsystem<UCommonUserSubsystem>())
		{
			// UserSubsystem->SendSystemMessage();
		}
	}
	if (SearchRequest->Results.IsEmpty())
	{
		NoGamesBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		NoGamesBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
	ListView->SetListItems(SearchRequest->Results);
	SetUserFocus(GetOwningPlayer());
	ListView->SetUserFocus(GetOwningPlayer());
	UnlockAfterSearchFinish();
}
