#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/IInputProcessor.h"
#include "Runtime/Engine/Public/Subsystems/GameInstanceSubsystem.h"
#include "LoadingScreenManager.generated.h"

class FLoadingScreenInputPreProcessor : public IInputProcessor
{
public:
	FLoadingScreenInputPreProcessor()
	{
	}

	virtual ~FLoadingScreenInputPreProcessor()
	{
	}

	bool CanEatInput() const
	{
		return !GIsEditor;
	}

	//~IInputProcess interface
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
	};

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		return CanEatInput();
	}

	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		return CanEatInput();
	}

	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp,
	                                    const FAnalogInputEvent& InAnalogInputEvent) override
	{
		return CanEatInput();
	}

	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		return CanEatInput();
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		return CanEatInput();
	}

	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		return CanEatInput();
	}

	virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp,
	                                               const FPointerEvent& MouseEvent) override
	{
		return CanEatInput();
	}

	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent,
	                                            const FPointerEvent* InGestureEvent) override
	{
		return CanEatInput();
	}

	virtual bool HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent) override
	{
		return CanEatInput();
	}

	//~End of IInputProcess interface
};

UCLASS()
class LOADINGSCREEN_API ULoadingScreenManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	//~ USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End of USubsystem interface

	//~ FTickableGameObject interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	//~ End of FTickableGameObject interface
protected:
	UFUNCTION()
	void HandlePreLoadMap(const FWorldContext& InContext, const FString& MapName);
	UFUNCTION()
	void HandlePostLoadMap(UWorld* World);

	bool IsShowingInitialLoadingScreen() const;

	bool ShouldShowLoadingScreen();
	bool CheckForAnyNeedToShowLoadingScreen();
	void UpdateLoadingScreen();

	void ShowLoadingScreen();
	void HideLoadingScreen();

	void RemoveWidgetFromViewport();

	void StartBlockingInput();
	void StopBlockingInput();

	void ChangePerformanceSettings(bool bEnabingLoadingScreen);

private:
	TSharedPtr<SWidget> LoadingScreenWidget;

	TSharedPtr<IInputProcessor> InputPreProcessor;

	/* 当前是否正在加载地图，即在过程 PreLoadMap 和 PostLoadMap之间 */
	bool bCurrentlyInLoadMap = false;
	/* 当前是否正在显示加载屏幕 */
	bool bCurrentlyShowingLoadingScreen = false;
};
