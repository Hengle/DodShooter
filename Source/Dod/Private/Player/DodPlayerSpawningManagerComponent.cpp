#include "Player/DodPlayerSpawningManagerComponent.h"

#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "GameFramework/PlayerState.h"
#include "Player/DodPlayerStart.h"


UDodPlayerSpawningManagerComponent::UDodPlayerSpawningManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(false);
	bAutoRegister = true;
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UDodPlayerSpawningManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &ThisClass::OnLevelAdded);

	UWorld* World = GetWorld();
	World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ThisClass::HandleOnActorSpawned));

	for (TActorIterator<ADodPlayerStart> It(World); It; ++It)
	{
		if (ADodPlayerStart* PlayerStart = *It)
		{
			CachedPlayerStarts.Add(PlayerStart);
		}
	}
}

void UDodPlayerSpawningManagerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor* UDodPlayerSpawningManagerComponent::ChoosePlayerStart(AController* Player)
{
	if (Player)
	{
#if WITH_EDITOR
		if (APlayerStart* PlayerStart = FindPlayFromHereStart(Player))
		{
			return PlayerStart;
		}
#endif

		TArray<ADodPlayerStart*> StarterPoints;
		for (auto StartIt = CachedPlayerStarts.CreateIterator(); StartIt; ++StartIt)
		{
			if (ADodPlayerStart* Start = (*StartIt).Get())
			{
				StarterPoints.Add(Start);
			}
			else
			{
				StartIt.RemoveCurrent();
			}
		}

		if (APlayerState* PlayerState = Player->GetPlayerState<APlayerState>())
		{
			// start dedicated spectators at any random starting location, but they do not claim it
			if (PlayerState->IsOnlyASpectator())
			{
				if (!StarterPoints.IsEmpty())
				{
					return StarterPoints[FMath::RandRange(0, StarterPoints.Num() - 1)];
				}

				return nullptr;
			}
		}

		AActor* PlayerStart = OnChoosePlayerStart(Player, StarterPoints);

		if (!PlayerStart)
		{
			PlayerStart = GetFirstRandomUnoccupiedPlayerStart(Player, StarterPoints);
		}

		if (ADodPlayerStart* DodStart = Cast<ADodPlayerStart>(PlayerStart))
		{
			DodStart->TryClaim(Player);
		}

		return PlayerStart;
	}

	return nullptr;
}

void UDodPlayerSpawningManagerComponent::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
}

bool UDodPlayerSpawningManagerComponent::ControllerCanRestart(AController* Player)
{
	return true;
}

APlayerStart* UDodPlayerSpawningManagerComponent::GetFirstRandomUnoccupiedPlayerStart(AController* Controller,
	const TArray<ADodPlayerStart*>& FoundStartPoints) const
{
	if (Controller)
	{
		TArray<ADodPlayerStart*> UnOccupiedStartPoints;
		TArray<ADodPlayerStart*> OccupiedStartPoints;

		for (ADodPlayerStart* StartPoint : FoundStartPoints)
		{
			EDodPlayerStartLocationOccupancy State = StartPoint->GetLocationOccupancy(Controller);

			switch (State)
			{
			case EDodPlayerStartLocationOccupancy::Empty:
				UnOccupiedStartPoints.Add(StartPoint);
				break;
			case EDodPlayerStartLocationOccupancy::Partial:
				OccupiedStartPoints.Add(StartPoint);
				break;
			default:
				break;
			}
		}

		if (UnOccupiedStartPoints.Num() > 0)
		{
			return UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
		}
		else if (OccupiedStartPoints.Num() > 0)
		{
			return OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}

	return nullptr;
}

void UDodPlayerSpawningManagerComponent::OnLevelAdded(ULevel* InLevel, UWorld* InWorld)
{
	if (InWorld == GetWorld())
	{
		for (AActor* Actor : InLevel->Actors)
		{
			if (ADodPlayerStart* PlayerStart = Cast<ADodPlayerStart>(Actor))
			{
				ensure(!CachedPlayerStarts.Contains(PlayerStart));
				CachedPlayerStarts.Add(PlayerStart);
			}
		}
	}
}

void UDodPlayerSpawningManagerComponent::HandleOnActorSpawned(AActor* SpawnedActor)
{
	if (ADodPlayerStart* PlayerStart = Cast<ADodPlayerStart>(SpawnedActor))
	{
		CachedPlayerStarts.Add(PlayerStart);
	}
}

#if WITH_EDITOR
APlayerStart* UDodPlayerSpawningManagerComponent::FindPlayFromHereStart(AController* Player)
{
	if (Player->IsA<APlayerController>())
	{
		if (UWorld* World = GetWorld())
		{
			for (TActorIterator<APlayerStart> It(World); It; ++It)
			{
				if (APlayerStart* PlayerStart = *It)
				{
					if (PlayerStart->IsA<APlayerStartPIE>())
					{
						return PlayerStart;
					}
				}
			}
		}
	}

	return nullptr;
}
#endif
