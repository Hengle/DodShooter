#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonUIExtensions.h"
#include "PrimaryGameLayout.generated.h"

struct FGameplayTag;
struct FStreamableHandle;
class UCommonActivatableWidget;

enum class EAsyncWidgetLayerState : uint8
{
	Canceled,
	Initialize,
	AfterPush
};

UCLASS()
class COMMONGAME_API UPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	static UPrimaryGameLayout* GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject);
	static UPrimaryGameLayout* GetPrimaryGameLayout(APlayerController* PlayerController);
	static UPrimaryGameLayout* GetPrimaryGameLayout(ULocalPlayer* LocalPlayer);

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete,
	                                                          TSoftClassPtr<UCommonActivatableWidget>
	                                                          ActivatableWidgetClass);

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete,
	                                                          TSoftClassPtr<UCommonActivatableWidget>
	                                                          ActivatableWidgetClass,
	                                                          TFunction<void(
		                                                          EAsyncWidgetLayerState,
		                                                          ActivatableWidgetT*)> StateFunc);

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass);

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass,
	                                           TFunctionRef<void(ActivatableWidgetT&)> InitInstanceFunc);

	UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag LayerName);

protected:
	UFUNCTION(BlueprintCallable, Category="Layer")
	void RegisterLayer(UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag,
	                   UCommonActivatableWidgetContainerBase* LayerWidget);

	void OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* Widget, bool bIsTransitioning);

private:
	TArray<FName> SuspendInputTokens;

	UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};

template <typename ActivatableWidgetT>
TSharedPtr<FStreamableHandle> UPrimaryGameLayout::PushWidgetToLayerStackAsync(FGameplayTag LayerName,
                                                                              bool bSuspendInputUntilComplete,
                                                                              TSoftClassPtr<UCommonActivatableWidget>
                                                                              ActivatableWidgetClass)
{
	return PushWidgetToLayerStackAsync<ActivatableWidgetT>(LayerName, bSuspendInputUntilComplete,
	                                                       ActivatableWidgetClass,
	                                                       [](EAsyncWidgetLayerState, ActivatableWidgetT*)
	                                                       {
	                                                       });
}

template <typename ActivatableWidgetT>
TSharedPtr<FStreamableHandle> UPrimaryGameLayout::PushWidgetToLayerStackAsync(FGameplayTag LayerName,
                                                                              bool bSuspendInputUntilComplete,
                                                                              TSoftClassPtr<UCommonActivatableWidget>
                                                                              ActivatableWidgetClass,
                                                                              TFunction<void(
	                                                                              EAsyncWidgetLayerState,
	                                                                              ActivatableWidgetT*)> StateFunc)
{
	static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived,
	              "Only CommonActivatableWidgets can be used here");

	static FName NAME_PushingWidgetToLayer("PushingWidgetToLayer");
	const FName SuspendInputToken = bSuspendInputUntilComplete
		                                ? UCommonUIExtensions::SuspendInputForPlayer(
			                                GetOwningPlayer(), NAME_PushingWidgetToLayer)
		                                : NAME_None;

	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
	TSharedPtr<FStreamableHandle> StreamingHandle = StreamableManager.RequestAsyncLoad(
		ActivatableWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this,
			[this, LayerName, ActivatableWidgetClass, StateFunc, SuspendInputToken]()
			{
				UCommonUIExtensions::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);

				ActivatableWidgetT* Widget = PushWidgetToLayerStack<ActivatableWidgetT>(
					LayerName, ActivatableWidgetClass.Get(), [StateFunc](ActivatableWidgetT& WidgetToInit)
					{
						StateFunc(EAsyncWidgetLayerState::Initialize, &WidgetToInit);
					});

				StateFunc(EAsyncWidgetLayerState::AfterPush, Widget);
			})
	);

	StreamingHandle->BindCancelDelegate(FStreamableDelegate::CreateWeakLambda(this,
	                                                                          [this, StateFunc, SuspendInputToken]()
	                                                                          {
		                                                                          UCommonUIExtensions::ResumeInputForPlayer(
			                                                                          GetOwningPlayer(),
			                                                                          SuspendInputToken);
		                                                                          StateFunc(
			                                                                          EAsyncWidgetLayerState::Canceled,
			                                                                          nullptr);
	                                                                          })
	);

	return StreamingHandle;
}

template <typename ActivatableWidgetT>
ActivatableWidgetT* UPrimaryGameLayout::PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass)
{
	return PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass, [](ActivatableWidgetT&)
	{
	});
}

template <typename ActivatableWidgetT>
ActivatableWidgetT* UPrimaryGameLayout::PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass,
                                                               TFunctionRef<void(ActivatableWidgetT&)> InitInstanceFunc)
{
	if (UCommonActivatableWidgetContainerBase* Layer = GetLayerWidget(LayerName))
	{
		return Layer->AddWidget<ActivatableWidgetT>(ActivatableWidgetClass, InitInstanceFunc);
	}

	return nullptr;
}
