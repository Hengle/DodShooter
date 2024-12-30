#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameUIPolicy.generated.h"

class UPrimaryGameLayout;
class UCommonLocalPlayer;

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPrimaryGameLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	FRootViewportLayoutInfo()
	{
	}

	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		  , RootLayout(InRootLayout)
		  , bAddedToViewport(bIsInViewport)
	{
	}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }
};

UCLASS(Blueprintable, Within = GameUIManagerSubsystem)
class COMMONGAME_API UGameUIPolicy : public UObject
{
	GENERATED_BODY()

public:
	UPrimaryGameLayout* GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const;

protected:
	void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	void CreateLayoutWidget(ULocalPlayer* LocalPlayer);

private:
	friend class UGameUIManagerSubsystem;

	void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> LayoutClass;
};
