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
