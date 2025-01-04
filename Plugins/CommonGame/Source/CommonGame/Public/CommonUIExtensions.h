#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonUIExtensions.generated.h"

struct FGameplayTag;
class UCommonActivatableWidget;

UCLASS()
class COMMONGAME_API UCommonUIExtensions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static UCommonActivatableWidget* PushContentToLayer_ForPlayer(
		const ULocalPlayer* LocalPlayer,
		UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName,
		UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UCommonActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static FName SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason);

	static FName SuspendInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendReason);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static void ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken);

	static void ResumeInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendToken);

private:
	static int32 InputSuspensions;
};
