#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/CancellableAsyncAction.h"
#include "AsyncAction_PushContentToLayerForPlayer.generated.h"

struct FStreamableHandle;
class UCommonActivatableWidget;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushContentToLayerForPlayerAsyncDelegate,
                                            UCommonActivatableWidget*, UserWidget);

UCLASS()
class COMMONGAME_API UAsyncAction_PushContentToLayerForPlayer : public UCancellableAsyncAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic,
		meta=(WorldContext = "WorldContextObject", BlueprintInternalUseOnly="true"))
	static UAsyncAction_PushContentToLayerForPlayer* PushContentToLayerForPlayer(
		APlayerController* OwningPlayer,
		UPARAM(meta = (AllowAbstract=false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass,
		UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName,
		bool bSuspendInputUntilComplete = true);

	virtual void Activate() override;
	virtual void Cancel() override;

	UPROPERTY(BlueprintAssignable)
	FPushContentToLayerForPlayerAsyncDelegate BeforePush;

	UPROPERTY(BlueprintAssignable)
	FPushContentToLayerForPlayerAsyncDelegate AfterPush;

private:
	TWeakObjectPtr<APlayerController> OwningPlayerPtr;
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass;
	FGameplayTag LayerName;
	bool bSuspendInputUntilComplete = false;
	
	TSharedPtr<FStreamableHandle> StreamingHandle;
};
