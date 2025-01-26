#include "Team/DodTeamAgentInterface.h"

void IDodTeamAgentInterface::ConditionalBroadcastTeamChanged(TScriptInterface<IDodTeamAgentInterface> This,
	FGenericTeamId OldTeamID, FGenericTeamId NewTeamID)
{
	if (OldTeamID != NewTeamID)
	{
		const int32 OldTeamIndex = GenericTeamIdToInteger(OldTeamID); 
		const int32 NewTeamIndex = GenericTeamIdToInteger(NewTeamID);

		UObject* ThisObj = This.GetObject();
		
		This.GetInterface()->GetTeamChangedDelegateChecked().Broadcast(ThisObj, OldTeamIndex, NewTeamIndex);
	}
}
