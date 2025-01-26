#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "UObject/Interface.h"
#include "DodTeamAgentInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDodTeamIndexChangedDelegate, UObject*, ObjectChangingTeam, int32,
                                               OldTeamID, int32, NewTeamID);

inline int32 GenericTeamIdToInteger(FGenericTeamId ID)
{
	return (ID == FGenericTeamId::NoTeam) ? INDEX_NONE : (int32)ID;
}

inline FGenericTeamId IntegerToGenericTeamId(int32 ID)
{
	return (ID == INDEX_NONE) ? FGenericTeamId::NoTeam : FGenericTeamId((uint8)ID);
}

UINTERFACE()
class UDodTeamAgentInterface : public UInterface
{
	GENERATED_BODY()
};

class DOD_API IDodTeamAgentInterface : public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	virtual FOnDodTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() { return nullptr; }

	static void ConditionalBroadcastTeamChanged(TScriptInterface<IDodTeamAgentInterface> This,
	                                            FGenericTeamId OldTeamID, FGenericTeamId NewTeamID);

	FOnDodTeamIndexChangedDelegate& GetTeamChangedDelegateChecked()
	{
		FOnDodTeamIndexChangedDelegate* Result = GetOnTeamIndexChangedDelegate();
		check(Result);
		return *Result;
	}
};
