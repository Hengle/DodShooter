#include "Team/DodTeamSubsystem.h"

#include "GenericTeamAgentInterface.h"
#include "Player/DodPlayerState.h"
#include "Team/DodTeamInfoBase.h"

void FDodTeamTrackingInfo::SetTeamInfo(ADodTeamInfoBase* Info)
{
}

void FDodTeamTrackingInfo::RemoveTeamInfo(ADodTeamInfoBase* Info)
{
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
	/*const FGenericTeamId NewTeamID = IntegerToGenericTeamId(NewTeamIndex);
	if (ADodPlayerState* DodPS = const_cast<ADodPlayerState*>(FindPlayerStateFromActor(ActorToChange)))
	{
		DodPS->SetGenericTeamId(NewTeamID);
		return true;
	}
	if (IDodTeamAgentInterface* TeamActor = Cast<IDodTeamAgentInterface>(ActorToChange))
	{
		TeamActor->SetGenericTeamId(NewTeamID);
		return true;
	}*/
	return false;
}

int32 UDodTeamSubsystem::FindTeamFromObject(const UObject* TestObject) const
{
	/*// See if it's directly a team agent
	if (const ILyraTeamAgentInterface* ObjectWithTeamInterface = Cast<ILyraTeamAgentInterface>(TestObject))
	{
		return GenericTeamIdToInteger(ObjectWithTeamInterface->GetGenericTeamId());
	}

	if (const AActor* TestActor = Cast<const AActor>(TestObject))
	{
		// See if the instigator is a team actor
		if (const ILyraTeamAgentInterface* InstigatorWithTeamInterface = Cast<ILyraTeamAgentInterface>(TestActor->GetInstigator()))
		{
			return GenericTeamIdToInteger(InstigatorWithTeamInterface->GetGenericTeamId());
		}

		// TeamInfo actors don't actually have the team interface, so they need a special case
		if (const ALyraTeamInfoBase* TeamInfo = Cast<ALyraTeamInfoBase>(TestActor))
		{
			return TeamInfo->GetTeamId();
		}

		// Fall back to finding the associated player state
		if (const ALyraPlayerState* LyraPS = FindPlayerStateFromActor(TestActor))
		{
			return LyraPS->GetTeamId();
		}
	}*/

	return INDEX_NONE;
}
