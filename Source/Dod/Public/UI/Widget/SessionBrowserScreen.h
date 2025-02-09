#pragma once

#include "CoreMinimal.h"
#include "UI/DodActivatableWidget.h"
#include "SessionBrowserScreen.generated.h"

class UCommonSession_SearchSessionRequest;
class UBorder;
class UDodListView;

UCLASS()
class DOD_API USessionBrowserScreen : public UDodActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnActivated() override;

protected:
	UFUNCTION(BlueprintCallable)
	void StartSearch();

	void UnlockAfterSearchFinish();

	UFUNCTION()
	void OnSearchFinished(bool bSucceeded, const FText& ErrorMessage);

	UPROPERTY(meta=(BindWidget))
	UDodListView* ListView;

	UPROPERTY(meta=(BindWidget))
	UBorder* NoGamesBorder;

private:
	UPROPERTY()
	UCommonSession_SearchSessionRequest* SearchRequest;

	bool bSearchLock{false};
};
