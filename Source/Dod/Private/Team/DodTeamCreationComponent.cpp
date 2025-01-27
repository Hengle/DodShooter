#include "Team/DodTeamCreationComponent.h"

#include "GameMode/DodGameMode.h"
#include "Player/DodPlayerState.h"
#include "Team/DodTeamDisplayAsset.h"
#include "Team/DodTeamPrivateInfo.h"
#include "Team/DodTeamPublicInfo.h"

UDodTeamCreationComponent::UDodTeamCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamsToCreate.Add(1, NewObject<UDodTeamDisplayAsset>());
	TeamsToCreate.Add(2, NewObject<UDodTeamDisplayAsset>());

	PublicTeamInfoClass = ADodTeamPublicInfo::StaticClass();
	PrivateTeamInfoClass = ADodTeamPrivateInfo::StaticClass();
}

void UDodTeamCreationComponent::BeginPlay()
{
	Super::BeginPlay();

#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		ServerCreateTeams();
		ServerAssignPlayersToTeams();
	}
#endif
}

#if WITH_SERVER_CODE
void UDodTeamCreationComponent::ServerCreateTeams()
{
	for (const auto& KVP : TeamsToCreate)
	{
		const int32 TeamId = KVP.Key;
		ServerCreateTeam(TeamId, KVP.Value);
	}
}

void UDodTeamCreationComponent::ServerAssignPlayersToTeams()
{
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (ADodPlayerState* DodPS = Cast<ADodPlayerState>(PS))
		{
			ServerChooseTeamForPlayer(DodPS);
		}
	}

	// Listen for new players logging in
	ADodGameMode* GameMode = Cast<ADodGameMode>(GameState->AuthorityGameMode);
	check(GameMode);

	GameMode->OnGameModePlayerInitialized.AddUObject(this, &ThisClass::OnPlayerInitialized);
}

void UDodTeamCreationComponent::ServerChooseTeamForPlayer(ADodPlayerState* PS)
{
	if (PS->IsOnlyASpectator())
	{
		PS->SetGenericTeamId(FGenericTeamId::NoTeam);
	}
	else
	{
		const FGenericTeamId TeamID = IntegerToGenericTeamId(GetLeastPopulatedTeamID());
		PS->SetGenericTeamId(TeamID);
	}
}

void UDodTeamCreationComponent::OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer)
{
	check(NewPlayer);
	check(NewPlayer->PlayerState);
	if (ADodPlayerState* DodPS = Cast<ADodPlayerState>(NewPlayer->PlayerState))
	{
		ServerChooseTeamForPlayer(DodPS);
	}
}

void UDodTeamCreationComponent::ServerCreateTeam(int32 TeamId, UDodTeamDisplayAsset* DisplayAsset)
{
	check(HasAuthority());

	//@TODO: ensure the team doesn't already exist

	UWorld* World = GetWorld();
	check(World);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADodTeamPublicInfo* NewTeamPublicInfo = World->SpawnActor<ADodTeamPublicInfo>(PublicTeamInfoClass, SpawnInfo);
	checkf(NewTeamPublicInfo != nullptr, TEXT("Failed to create public team actor from class %s"),
	       *GetPathNameSafe(*PublicTeamInfoClass));
	NewTeamPublicInfo->SetTeamId(TeamId);
	NewTeamPublicInfo->SetTeamDisplayAsset(DisplayAsset);

	ADodTeamPrivateInfo* NewTeamPrivateInfo = World->SpawnActor<ADodTeamPrivateInfo>(PrivateTeamInfoClass, SpawnInfo);
	checkf(NewTeamPrivateInfo != nullptr, TEXT("Failed to create private team actor from class %s"),
	       *GetPathNameSafe(*PrivateTeamInfoClass));
	NewTeamPrivateInfo->SetTeamId(TeamId);
}

int32 UDodTeamCreationComponent::GetLeastPopulatedTeamID() const
{
	const int32 NumTeams = TeamsToCreate.Num();
	if (NumTeams > 0)
	{
		TMap<int32, uint32> TeamMemberCounts;
		TeamMemberCounts.Reserve(NumTeams);

		for (const auto& KVP : TeamsToCreate)
		{
			const int32 TeamId = KVP.Key;
			TeamMemberCounts.Add(TeamId, 0);
		}

		AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
		for (APlayerState* PS : GameState->PlayerArray)
		{
			if (ADodPlayerState* DodPS = Cast<ADodPlayerState>(PS))
			{
				const int32 PlayerTeamID = DodPS->GetTeamId();

				if ((PlayerTeamID != INDEX_NONE) && !DodPS->IsInactive())
				// do not count unassigned or disconnected players
				{
					check(TeamMemberCounts.Contains(PlayerTeamID))
					TeamMemberCounts[PlayerTeamID] += 1;
				}
			}
		}

		// sort by lowest team population, then by team ID
		int32 BestTeamId = INDEX_NONE;
		uint32 BestPlayerCount = TNumericLimits<uint32>::Max();
		for (const auto& KVP : TeamMemberCounts)
		{
			const int32 TestTeamId = KVP.Key;
			const uint32 TestTeamPlayerCount = KVP.Value;

			if (TestTeamPlayerCount < BestPlayerCount)
			{
				BestTeamId = TestTeamId;
				BestPlayerCount = TestTeamPlayerCount;
			}
			else if (TestTeamPlayerCount == BestPlayerCount)
			{
				if ((TestTeamId < BestTeamId) || (BestTeamId == INDEX_NONE))
				{
					BestTeamId = TestTeamId;
					BestPlayerCount = TestTeamPlayerCount;
				}
			}
		}

		return BestTeamId;
	}

	return INDEX_NONE;
}
#endif
