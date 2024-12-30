#include "LoadingScreenManager.h"

#include "CommonLoadingScreenSettings.h"
#include "LoadingProcessInterface.h"
#include "PreLoadScreenManager.h"
#include "ShaderPipelineCache.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameStateBase.h"
#include "HAL/ThreadHeartBeat.h"
#include "Widgets/Images/SThrobber.h"

void ULoadingScreenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::HandlePreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::HandlePostLoadMap);
}

void ULoadingScreenManager::Deinitialize()
{
	Super::Deinitialize();

	RemoveWidgetFromViewport();

	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

bool ULoadingScreenManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void ULoadingScreenManager::Tick(float DeltaTime)
{
	UpdateLoadingScreen();
}

TStatId ULoadingScreenManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULoadingScreenManager, STATGROUP_Tickables);
}

void ULoadingScreenManager::HandlePreLoadMap(const FWorldContext& InContext, const FString& MapName)
{
	if (InContext.OwningGameInstance == GetGameInstance())
	{
		bCurrentlyInLoadMap = true;

		if (GEngine->IsInitialized())
		{
			UpdateLoadingScreen();
		}
	}
}

void ULoadingScreenManager::HandlePostLoadMap(UWorld* World)
{
	if (World && World->GetGameInstance() == GetGameInstance())
	{
		bCurrentlyInLoadMap = false;
	}
}

bool ULoadingScreenManager::IsShowingInitialLoadingScreen() const
{
	FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get();
	return PreLoadScreenManager && PreLoadScreenManager->HasValidActivePreLoadScreen();
}

bool ULoadingScreenManager::ShouldShowLoadingScreen()
{
	const bool bNeedToShowLoadingScreen = CheckForAnyNeedToShowLoadingScreen();

	return bNeedToShowLoadingScreen;
}

bool ULoadingScreenManager::CheckForAnyNeedToShowLoadingScreen()
{
	const UGameInstance* LocalGameInstance = GetGameInstance();
	if (!LocalGameInstance)
	{
		return false;
	}

	const FWorldContext* Context = LocalGameInstance->GetWorldContext();
	if (!Context)
	{
		// The game instance has a null WorldContext
		return true;
	}

	UWorld* World = Context->World();
	if (!World)
	{
		// "We have no world (FWorldContext's World() is null)"
		return true;
	}

	AGameStateBase* GameState = World->GetGameState<AGameStateBase>();
	if (!GameState)
	{
		// The game state has not yet replicated.
		return true;
	}

	if (bCurrentlyInLoadMap)
	{
		// Show a loading screen if we are in LoadMap
		return true;
	}

	if (!Context->TravelURL.IsEmpty())
	{
		// Show a loading screen when pending travel
		return true;
	}

	if (Context->PendingNetGame != nullptr)
	{
		// Connecting to another server
		return true;
	}

	if (!World->HasBegunPlay())
	{
		// World hasn't begun play
		return true;
	}

	if (World->IsInSeamlessTravel())
	{
		// Show a loading screen during seamless travel
		return true;
	}

	// Ask the game state if it needs a loading screen	
	if (ILoadingProcessInterface::ShouldShowLoadingScreen(GameState))
	{
		return true;
	}

	// Ask any game state components if they need a loading screen
	for (UActorComponent* TestComponent : GameState->GetComponents())
	{
		if (ILoadingProcessInterface::ShouldShowLoadingScreen(TestComponent))
		{
			return true;
		}
	}

	// Check each local player
	bool bFoundAnyLocalPC = false;
	bool bMissingAnyLocalPC = false;

	for (ULocalPlayer* LP : LocalGameInstance->GetLocalPlayers())
	{
		if (!LP)
		{
			continue;
		}
		if (APlayerController* PC = LP->PlayerController)
		{
			bFoundAnyLocalPC = true;

			// Ask the PC itself if it needs a loading screen
			if (ILoadingProcessInterface::ShouldShowLoadingScreen(PC))
			{
				return true;
			}

			// Ask any PC components if they need a loading screen
			for (UActorComponent* TestComponent : PC->GetComponents())
			{
				if (ILoadingProcessInterface::ShouldShowLoadingScreen(TestComponent))
				{
					return true;
				}
			}
		}
		else
		{
			bMissingAnyLocalPC = true;
		}
	}

	UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient();
	const bool bIsInSplitscreen = GameViewportClient->GetCurrentSplitscreenConfiguration() != ESplitScreenType::None;

	// In splitscreen we need all player controllers to be present
	if (bIsInSplitscreen && bMissingAnyLocalPC)
	{
		// At least one missing local player controller in splitscreen
		return true;
	}

	// And in non-splitscreen we need at least one player controller to be present
	if (!bIsInSplitscreen && !bFoundAnyLocalPC)
	{
		// Need at least one local player controller
		return true;
	}
	return false;
}

void ULoadingScreenManager::UpdateLoadingScreen()
{
	if (ShouldShowLoadingScreen())
	{
		ShowLoadingScreen();
	}
	else
	{
		HideLoadingScreen();
	}
}

void ULoadingScreenManager::ShowLoadingScreen()
{
	if (bCurrentlyShowingLoadingScreen)
	{
		return;
	}

	if (FPreLoadScreenManager::Get() && FPreLoadScreenManager::Get()->HasActivePreLoadScreenType(
		EPreLoadScreenTypes::EngineLoadingScreen))
	{
		return;
	}

	bCurrentlyShowingLoadingScreen = true;

	const UCommonLoadingScreenSettings* Settings = GetDefault<UCommonLoadingScreenSettings>();

	if (!IsShowingInitialLoadingScreen())
	{
		UGameInstance* LocalGameInstance = GetGameInstance();

		// Eat input while the loading screen is displayed
		StartBlockingInput();

		// Create the loading screen widget
		TSubclassOf<UUserWidget> LoadingScreenWidgetClass = Settings->LoadingScreenWidget.TryLoadClass<UUserWidget>();
		if (UUserWidget* UserWidget = UUserWidget::CreateWidgetInstance(*LocalGameInstance, LoadingScreenWidgetClass,
		                                                                NAME_None))
		{
			LoadingScreenWidget = UserWidget->TakeWidget();
		}
		else
		{
			// Failed to load the loading screen widget
			LoadingScreenWidget = SNew(SThrobber);
		}

		// Add to the viewport at a high ZOrder to make sure it is on top of most things
		UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient();
		GameViewportClient->AddViewportWidgetContent(LoadingScreenWidget.ToSharedRef(), Settings->LoadingScreenZOrder);

		ChangePerformanceSettings(/*bEnableLoadingScreen=*/ true);

		if (!GIsEditor)
		{
			// Tick Slate to make sure the loading screen is displayed immediately
			FSlateApplication::Get().Tick();
		}
	}
}

void ULoadingScreenManager::HideLoadingScreen()
{
	if (!bCurrentlyShowingLoadingScreen)
	{
		return;
	}

	StopBlockingInput();

	if (!IsShowingInitialLoadingScreen())
	{
		GEngine->ForceGarbageCollection(true);

		RemoveWidgetFromViewport();

		ChangePerformanceSettings(/*bEnableLoadingScreen=*/ false);
	}

	bCurrentlyShowingLoadingScreen = false;
}

void ULoadingScreenManager::RemoveWidgetFromViewport()
{
	UGameInstance* LocalGameInstance = GetGameInstance();
	if (LoadingScreenWidget.IsValid())
	{
		if (UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient())
		{
			GameViewportClient->RemoveViewportWidgetContent(LoadingScreenWidget.ToSharedRef());
		}
		LoadingScreenWidget.Reset();
	}
}

void ULoadingScreenManager::StartBlockingInput()
{
	if (!InputPreProcessor.IsValid())
	{
		InputPreProcessor = MakeShareable<FLoadingScreenInputPreProcessor>(new FLoadingScreenInputPreProcessor());
		FSlateApplication::Get().RegisterInputPreProcessor(InputPreProcessor, 0);
	}
}

void ULoadingScreenManager::StopBlockingInput()
{
	if (InputPreProcessor.IsValid())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputPreProcessor);
		InputPreProcessor.Reset();
	}
}

void ULoadingScreenManager::ChangePerformanceSettings(bool bEnabingLoadingScreen)
{
	UGameInstance* LocalGameInstance = GetGameInstance();
	UGameViewportClient* GameViewportClient = LocalGameInstance->GetGameViewportClient();

	FShaderPipelineCache::SetBatchMode(bEnabingLoadingScreen ? FShaderPipelineCache::BatchMode::Fast : FShaderPipelineCache::BatchMode::Background);

	// Don't bother drawing the 3D world while we're loading
	GameViewportClient->bDisableWorldRendering = bEnabingLoadingScreen;

	// Make sure to prioritize streaming in levels if the loading screen is up
	if (UWorld* ViewportWorld = GameViewportClient->GetWorld())
	{
		if (AWorldSettings* WorldSettings = ViewportWorld->GetWorldSettings(false, false))
		{
			WorldSettings->bHighPriorityLoadingLocal = bEnabingLoadingScreen;
		}
	}

	if (bEnabingLoadingScreen)
	{
		// Set a new hang detector timeout multiplier when the loading screen is visible.
		double HangDurationMultiplier;
		if (!GConfig || !GConfig->GetDouble(TEXT("Core.System"), TEXT("LoadingScreenHangDurationMultiplier"), /*out*/ HangDurationMultiplier, GEngineIni))
		{
			HangDurationMultiplier = 1.0;
		}
		FThreadHeartBeat::Get().SetDurationMultiplier(HangDurationMultiplier);

		// Do not report hitches while the loading screen is up
		FGameThreadHitchHeartBeat::Get().SuspendHeartBeat();
	}
	else
	{
		// Restore the hang detector timeout when we hide the loading screen
		FThreadHeartBeat::Get().SetDurationMultiplier(1.0);

		// Resume reporting hitches now that the loading screen is down
		FGameThreadHitchHeartBeat::Get().ResumeHeartBeat();
	}
}
