#include "Team/DodTeamSubsystem.h"

#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "Player/DodPlayerState.h"
#include "Team/DodTeamInfoBase.h"
#include "Team/DodTeamPrivateInfo.h"
#include "Team/DodTeamPublicInfo.h"

void FDodTeamTrackingInfo::SetTeamInfo(ADodTeamInfoBase* Info)
{
	if (ADodTeamPublicInfo* NewPublicInfo = Cast<ADodTeamPublicInfo>(Info))
	{
		ensure((PublicInfo == nullptr) || (PublicInfo == NewPublicInfo));
		PublicInfo = NewPublicInfo;

		UDodTeamDisplayAsset* OldDisplayAsset = DisplayAsset;
		DisplayAsset = NewPublicInfo->GetTeamDisplayAsset();

		if (OldDisplayAsset != DisplayAsset)
		{
			OnTeamDisplayAssetChanged.Broadcast(DisplayAsset);
		}
	}
	else if (ADodTeamPrivateInfo* NewPrivateInfo = Cast<ADodTeamPrivateInfo>(Info))
	{
		ensure((PrivateInfo == nullptr) || (PrivateInfo == NewPrivateInfo));
		PrivateInfo = NewPrivateInfo;
	}
	else
	{
		checkf(false, TEXT("Expected a public or private team info but got %s"), *GetPathNameSafe(Info))
	}
}

void FDodTeamTrackingInfo::RemoveTeamInfo(ADodTeamInfoBase* Info)
{
	if (PublicInfo == Info)
	{
		PublicInfo = nullptr;
	}
	else if (PrivateInfo == Info)
	{
		PrivateInfo = nullptr;
	}
	else
	{
		ensureMsgf(false, TEXT("Expected a previously registered team info but got %s"), *GetPathNameSafe(Info));
	}
}

bool UDodTeamSubsystem::RegisterTeamInfo(ADodTeamInfoBase* TeamInfo)
{
	if (!ensure(TeamInfo))
	{
		return false;
	}

	const int32 TeamId = TeamInfo->GetTeamId();
	if (ensure(TeamId != INDEX_NONE))
	{
		FDodTeamTrackingInfo& Entry = TeamMap.FindOrAdd(TeamId);
		Entry.SetTeamInfo(TeamInfo);

		return true;
	}

	return false;
}

bool UDodTeamSubsystem::UnregisterTeamInfo(ADodTeamInfoBase* TeamInfo)
{
	if (!ensure(TeamInfo))
	{
		return false;
	}
	const int32 TeamId = TeamInfo->GetTeamId();
	if (ensure(TeamId != INDEX_NONE))
	{
		FDodTeamTrackingInfo* Entry = TeamMap.Find(TeamId);
		if (Entry)
		{
			Entry->RemoveTeamInfo(TeamInfo);
			return true;
		}
	}
	return false;
}

bool UDodTeamSubsystem::ChangeTeamForActor(AActor* ActorToChange, int32 NewTeamIndex)
{
	const FGenericTeamId NewTeamID = IntegerToGenericTeamId(NewTeamIndex);
	if (ADodPlayerState* DodPS = const_cast<ADodPlayerState*>(FindPlayerStateFromActor(ActorToChange)))
	{
		DodPS->SetGenericTeamId(NewTeamID);
		return true;
	}
	if (IDodTeamAgentInterface* TeamActor = Cast<IDodTeamAgentInterface>(ActorToChange))
	{
		TeamActor->SetGenericTeamId(NewTeamID);
		return true;
	}
	return false;
}

int32 UDodTeamSubsystem::FindTeamFromObject(const UObject* TestObject) const
{
	// See if it's directly a team agent
	if (const IDodTeamAgentInterface* ObjectWithTeamInterface = Cast<IDodTeamAgentInterface>(TestObject))
	{
		return GenericTeamIdToInteger(ObjectWithTeamInterface->GetGenericTeamId());
	}

	if (const AActor* TestActor = Cast<const AActor>(TestObject))
	{
		// See if the instigator is a team actor
		if (const IDodTeamAgentInterface* InstigatorWithTeamInterface = Cast<IDodTeamAgentInterface>(
			TestActor->GetInstigator()))
		{
			return GenericTeamIdToInteger(InstigatorWithTeamInterface->GetGenericTeamId());
		}

		// TeamInfo actors don't actually have the team interface, so they need a special case
		if (const ADodTeamInfoBase* TeamInfo = Cast<ADodTeamInfoBase>(TestActor))
		{
			return TeamInfo->GetTeamId();
		}

		// Fall back to finding the associated player state
		if (const ADodPlayerState* DodPS = FindPlayerStateFromActor(TestActor))
		{
			return DodPS->GetTeamId();
		}
	}

	return INDEX_NONE;
}

const ADodPlayerState* UDodTeamSubsystem::FindPlayerStateFromActor(const AActor* PossibleTeamActor) const
{
	if (PossibleTeamActor != nullptr)
	{
		if (const APawn* Pawn = Cast<const APawn>(PossibleTeamActor))
		{
			//@TODO: Consider an interface instead or have team actors register with the subsystem and have it maintain a map? (or LWC style)
			if (ADodPlayerState* LyraPS = Pawn->GetPlayerState<ADodPlayerState>())
			{
				return LyraPS;
			}
		}
		else if (const AController* PC = Cast<const AController>(PossibleTeamActor))
		{
			if (ADodPlayerState* LyraPS = Cast<ADodPlayerState>(PC->PlayerState))
			{
				return LyraPS;
			}
		}
		else if (const ADodPlayerState* LyraPS = Cast<const ADodPlayerState>(PossibleTeamActor))
		{
			return LyraPS;
		}
	}

	return nullptr;
}

EDodTeamComparison UDodTeamSubsystem::CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA,
                                                   int32& TeamIdB) const
{
	TeamIdA = FindTeamFromObject(Cast<const AActor>(A));
	TeamIdB = FindTeamFromObject(Cast<const AActor>(B));

	if (TeamIdA == INDEX_NONE || TeamIdB == INDEX_NONE)
	{
		return EDodTeamComparison::InvalidArgument;
	}
	return TeamIdA == TeamIdB ? EDodTeamComparison::OnSameTeam : EDodTeamComparison::DifferentTeams;
}

EDodTeamComparison UDodTeamSubsystem::CompareTeams(const UObject* A, const UObject* B) const
{
	int32 TeamIdA;
	int32 TeamIdB;
	return CompareTeams(A, B, TeamIdA, TeamIdB);
}

void UDodTeamSubsystem::AddTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount)
{
	if (FDodTeamTrackingInfo* Entry = TeamMap.Find(TeamId))
	{
		if (Entry->PublicInfo)
		{
			if (Entry->PublicInfo->HasAuthority())
			{
				Entry->PublicInfo->TeamTags.AddStack(Tag, StackCount);
			}
		}
	}
}

int32 UDodTeamSubsystem::GetTeamTagStackCount(int32 TeamId, FGameplayTag Tag) const
{
	if (const FDodTeamTrackingInfo* Entry = TeamMap.Find(TeamId))
	{
		const int32 PublicStackCount = (Entry->PublicInfo != nullptr)
			                               ? Entry->PublicInfo->TeamTags.GetStackCount(Tag)
			                               : 0;
		const int32 PrivateStackCount = (Entry->PrivateInfo != nullptr)
			                                ? Entry->PrivateInfo->TeamTags.GetStackCount(Tag)
			                                : 0;
		return PublicStackCount + PrivateStackCount;
	}
	return 0;
}
