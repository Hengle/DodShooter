#include "GameMode/DodGameMode.h"

#include "CommonSessionSubsystem.h"
#include "CommonUserSubsystem.h"
#include "DodLogChannels.h"
#include "GameMapsSettings.h"
#include "Character/DodCharacter.h"
#include "Character/Comp/DodPawnExtensionComponent.h"
#include "GameMode/DodExperienceDefinition.h"
#include "GameMode/DodExperienceManagerComponent.h"
#include "GameMode/DodUserFacingExperienceDefinition.h"
#include "GameMode/DodWorldSetting.h"
#include "GameMode/GameState/DodGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DodPlayerBotController.h"
#include "Player/DodPlayerController.h"
#include "Player/DodPlayerSpawningManagerComponent.h"
#include "Player/DodPlayerState.h"
#include "System/DodAssetManager.h"
#include "UI/DodHUD.h"
#include "Character/DodPawnData.h"

ADodGameMode::ADodGameMode(const FObjectInitializer& ObjectInitializer)
{
	GameStateClass = ADodGameState::StaticClass();
	PlayerControllerClass = ADodPlayerController::StaticClass();
	PlayerStateClass = ADodPlayerState::StaticClass();
	DefaultPawnClass = ADodCharacter::StaticClass();
	HUDClass = ADodHUD::StaticClass();
}

const UDodPawnData* ADodGameMode::GetPawnDataForController(const AController* InController) const
{
	// See if pawn data is already set on the player state
	if (InController != nullptr)
	{
		if (const ADodPlayerState* DodPS = InController->GetPlayerState<ADodPlayerState>())
		{
			if (const UDodPawnData* PawnData = DodPS->GetPawnData<UDodPawnData>())
			{
				return PawnData;
			}
		}
	}

	// If not, fall back to the the default for the current experience
	check(GameState);
	UDodExperienceManagerComponent* ExperienceComponent =
		GameState->FindComponentByClass<UDodExperienceManagerComponent>();
	check(ExperienceComponent);

	if (ExperienceComponent->IsExperienceLoaded())
	{
		const UDodExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}

		// Experience is loaded and there's still no pawn data, fall back to the default for now
		return UDodAssetManager::Get().GetDefaultPawnData();
	}

	// Experience not loaded yet, so there is no pawn data to be had
	return nullptr;
}

void ADodGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

UClass* ADodGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UDodPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ADodGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
                                                                const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (UDodPawnExtensionComponent* PawnExtComp = UDodPawnExtensionComponent::FindPawnExtensionComponent(
				SpawnedPawn))
			{
				if (const UDodPawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
				else
				{
					UE_LOG(LogDod, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."),
					       *GetNameSafe(SpawnedPawn));
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
	}
	return nullptr;
}

bool ADodGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void ADodGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

AActor* ADodGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (UDodPlayerSpawningManagerComponent* PlayerSpawningComponent =
		GameState->FindComponentByClass<UDodPlayerSpawningManagerComponent>())
	{
		return PlayerSpawningComponent->ChoosePlayerStart(Player);
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ADodGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	if (UDodPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<
		UDodPlayerSpawningManagerComponent>())
	{
		PlayerSpawningComponent->FinishRestartPlayer(NewPlayer, StartRotation);
	}

	Super::FinishRestartPlayer(NewPlayer, StartRotation);
}

bool ADodGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return ControllerCanRestart(Player);
}

void ADodGameMode::GenericPlayerInitialization(AController* C)
{
	Super::GenericPlayerInitialization(C);

	OnGameModePlayerInitialized.Broadcast(this, C);
}

void ADodGameMode::FailedToRestartPlayer(AController* NewPlayer)
{
	Super::FailedToRestartPlayer(NewPlayer);

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APlayerController* NewPC = Cast<APlayerController>(NewPlayer))
		{
			// If it's a player don't loop forever, maybe something changed and they can no longer restart if so stop trying.
			if (PlayerCanRestart(NewPC))
			{
				RequestPlayerRestartNextFrame(NewPlayer, false);
			}
			else
			{
				UE_LOG(LogDod, Verbose,
				       TEXT(
					       "FailedToRestartPlayer(%s) and PlayerCanRestart returned false, so we're not going to try again."
				       ), *GetPathNameSafe(NewPlayer));
			}
		}
		else
		{
			RequestPlayerRestartNextFrame(NewPlayer, false);
		}
	}
	else
	{
		UE_LOG(LogDod, Verbose, TEXT("FailedToRestartPlayer(%s) but there's no pawn class so giving up."),
		       *GetPathNameSafe(NewPlayer));
	}
}

void ADodGameMode::InitGameState()
{
	Super::InitGameState();

	UDodExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<
		UDodExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded(
		FOnDodExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

bool ADodGameMode::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	return true;
}

bool ADodGameMode::ControllerCanRestart(AController* Controller)
{
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (!Super::PlayerCanRestart_Implementation(PC))
		{
			return false;
		}
	}
	else
	{
		// Bot version of Super::PlayerCanRestart_Implementation
		if (Controller || Controller->IsPendingKillPending())
		{
			return false;
		}
	}

	if (UDodPlayerSpawningManagerComponent* PlayerSpawningComponent =
		GameState->FindComponentByClass<UDodPlayerSpawningManagerComponent>())
	{
		return PlayerSpawningComponent->ControllerCanRestart(Controller);
	}

	return true;
}

void ADodGameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if (bForceReset && Controller)
	{
		Controller->Reset();
	}

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PC, &APlayerController::ServerRestartPlayer_Implementation);
	}
	else if (ADodPlayerBotController* BotController = Cast<ADodPlayerBotController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(BotController, &ADodPlayerBotController::ServerRestartController);
	}
}

void ADodGameMode::OnExperienceLoaded(const UDodExperienceDefinition* CurrentExperience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

bool ADodGameMode::IsExperienceLoaded() const
{
	check(GameState);
	UDodExperienceManagerComponent* ExperienceComponent =
		GameState->FindComponentByClass<UDodExperienceManagerComponent>();
	check(ExperienceComponent);

	return ExperienceComponent->IsExperienceLoaded();
}

void ADodGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;

	UWorld* World = GetWorld();

	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UDodExperienceDefinition::StaticClass()->GetFName()),
		                               FName(*ExperienceFromOptions));
		ExperienceIdSource = TEXT("OptionsString");
	}

	// see if the command line wants to set the experience
	if (!ExperienceId.IsValid())
	{
		FString ExperienceFromCommandLine;
		if (FParse::Value(FCommandLine::Get(), TEXT("Experience="), ExperienceFromCommandLine))
		{
			ExperienceId = FPrimaryAssetId::ParseTypeAndName(ExperienceFromCommandLine);
			if (!ExperienceId.PrimaryAssetType.IsValid())
			{
				ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UDodExperienceDefinition::StaticClass()->GetFName()),
				                               FName(*ExperienceFromCommandLine));
			}
			ExperienceIdSource = TEXT("CommandLine");
		}
	}

	// see if the world settings has a default experience
	if (!ExperienceId.IsValid())
	{
		if (ADodWorldSetting* TypedWorldSettings = Cast<ADodWorldSetting>(GetWorldSettings()))
		{
			ExperienceId = TypedWorldSettings->GetDefaultGameplayExperience();
			ExperienceIdSource = TEXT("WorldSettings");
		}
	}

	UDodAssetManager& AssetManager = UDodAssetManager::Get();
	FAssetData Dummy;
	if (ExperienceId.IsValid() && !AssetManager.GetPrimaryAssetData(ExperienceId, /*out*/ Dummy))
	{
		UE_LOG(LogDodExperience, Error,
		       TEXT("EXPERIENCE: Wanted to use %s but couldn't find it, falling back to the default)"),
		       *ExperienceId.ToString());
		ExperienceId = FPrimaryAssetId();
	}

	// Final fallback to the default experience
	if (!ExperienceId.IsValid())
	{
		if (TryDedicatedServerLogin())
		{
			// This will start to host as a dedicated server
			return;
		}

		//@TODO: Pull this from a config setting or something
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("DodExperienceDefinition"), FName("BP_DodDefaultExperience"));
		ExperienceIdSource = TEXT("Default");
	}

	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}

void ADodGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
	if (ExperienceId.IsValid())
	{
		UE_LOG(LogDodExperience, Log, TEXT("Identified experience %s (Source: %s)"), *ExperienceId.ToString(),
		       *ExperienceIdSource);

		UDodExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<
			UDodExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->SetCurrentExperience(ExperienceId);
	}
	else
	{
		UE_LOG(LogDodExperience, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
}

bool ADodGameMode::TryDedicatedServerLogin()
{
	// Some basic code to register as an active dedicated server, this would be heavily modified by the game
	FString DefaultMap = UGameMapsSettings::GetGameDefaultMap();
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance && World && World->GetNetMode() == NM_DedicatedServer && World->URL.Map == DefaultMap)
	{
		// Only register if this is the default map on a dedicated server
		UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();

		// Dedicated servers may need to do an online login
		UserSubsystem->OnUserInitializeComplete.AddDynamic(this, &ADodGameMode::OnUserInitializedForDedicatedServer);

		// There are no local users on dedicated server, but index 0 means the default platform user which is handled by the online login code
		if (!UserSubsystem->TryToLoginForOnlinePlay(0))
		{
			OnUserInitializedForDedicatedServer(nullptr, false, FText(), ECommonUserPrivilege::CanPlayOnline,
			                                    ECommonUserOnlineContext::Default);
		}

		return true;
	}

	return false;
}

void ADodGameMode::HostDedicatedServerMatch(ECommonSessionOnlineMode OnlineMode)
{
	FPrimaryAssetType UserExperienceType = UDodUserFacingExperienceDefinition::StaticClass()->GetFName();

	// Figure out what UserFacingExperience to load
	FPrimaryAssetId UserExperienceId;
	FString UserExperienceFromCommandLine;
	if (FParse::Value(FCommandLine::Get(), TEXT("UserExperience="), UserExperienceFromCommandLine) ||
		FParse::Value(FCommandLine::Get(), TEXT("Playlist="), UserExperienceFromCommandLine))
	{
		UserExperienceId = FPrimaryAssetId::ParseTypeAndName(UserExperienceFromCommandLine);
		if (!UserExperienceId.PrimaryAssetType.IsValid())
		{
			UserExperienceId = FPrimaryAssetId(FPrimaryAssetType(UserExperienceType),
			                                   FName(*UserExperienceFromCommandLine));
		}
	}

	// Search for the matching experience, it's fine to force load them because we're in dedicated server startup
	UDodAssetManager& AssetManager = UDodAssetManager::Get();
	TSharedPtr<FStreamableHandle> Handle = AssetManager.LoadPrimaryAssetsWithType(UserExperienceType);
	if (ensure(Handle.IsValid()))
	{
		Handle->WaitUntilComplete();
	}

	TArray<UObject*> UserExperiences;
	AssetManager.GetPrimaryAssetObjectList(UserExperienceType, UserExperiences);
	UDodUserFacingExperienceDefinition* FoundExperience = nullptr;
	UDodUserFacingExperienceDefinition* DefaultExperience = nullptr;

	for (UObject* Object : UserExperiences)
	{
		UDodUserFacingExperienceDefinition* UserExperience = Cast<UDodUserFacingExperienceDefinition>(Object);
		if (ensure(UserExperience))
		{
			if (UserExperience->GetPrimaryAssetId() == UserExperienceId)
			{
				FoundExperience = UserExperience;
				break;
			}

			if (UserExperience->bIsDefaultExperience && DefaultExperience == nullptr)
			{
				DefaultExperience = UserExperience;
			}
		}
	}

	if (FoundExperience == nullptr)
	{
		FoundExperience = DefaultExperience;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (ensure(FoundExperience && GameInstance))
	{
		// Actually host the game
		UCommonSession_HostSessionRequest* HostRequest = FoundExperience->CreateHostingRequest(this);
		if (ensure(HostRequest))
		{
			HostRequest->OnlineMode = OnlineMode;

			// TODO override other parameters?

			UCommonSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>();
			SessionSubsystem->HostSession(nullptr, HostRequest);

			// This will handle the map travel
		}
	}
}

void ADodGameMode::OnUserInitializedForDedicatedServer(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error,
                                                       ECommonUserPrivilege RequestedPrivilege,
                                                       ECommonUserOnlineContext OnlineContext)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		// Unbind
		UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();
		UserSubsystem->OnUserInitializeComplete.RemoveDynamic(this, &ADodGameMode::OnUserInitializedForDedicatedServer);

		// Dedicated servers do not require user login, but some online subsystems may expect it
		if (bSuccess && ensure(UserInfo))
		{
			UE_LOG(LogDodExperience, Log,
			       TEXT("Dedicated server user login succeeded for id %s, starting online server"),
			       *UserInfo->GetNetId().ToString());
		}
		else
		{
			UE_LOG(LogDodExperience, Log,
			       TEXT("Dedicated server user login unsuccessful, starting online server as login is not required"));
		}

		HostDedicatedServerMatch(ECommonSessionOnlineMode::Online);
	}
}
