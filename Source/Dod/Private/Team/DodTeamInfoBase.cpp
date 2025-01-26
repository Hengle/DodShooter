#include "Team/DodTeamInfoBase.h"

#include "Net/UnrealNetwork.h"
#include "Team/DodTeamSubsystem.h"


ADodTeamInfoBase::ADodTeamInfoBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bAlwaysRelevant = true;
	NetPriority = 3.0f;
	SetReplicatingMovement(false);
}

void ADodTeamInfoBase::SetTeamId(int32 NewTeamId)
{
	check(HasAuthority());
	check(TeamId == INDEX_NONE);
	check(NewTeamId != INDEX_NONE);

	TeamId = NewTeamId;

	TryRegisterWithTeamSubsystem();
}

void ADodTeamInfoBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamTags);
	DOREPLIFETIME_CONDITION(ThisClass, TeamId, COND_InitialOnly);
}

void ADodTeamInfoBase::BeginPlay()
{
	Super::BeginPlay();
	TryRegisterWithTeamSubsystem();
}

void ADodTeamInfoBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TeamId != INDEX_NONE)
	{
		UDodTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UDodTeamSubsystem>();
		if (TeamSubsystem)
		{
			TeamSubsystem->UnregisterTeamInfo(this);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void ADodTeamInfoBase::RegisterWithTeamSubsystem(UDodTeamSubsystem* Subsystem)
{
	Subsystem->RegisterTeamInfo(this);
}

void ADodTeamInfoBase::TryRegisterWithTeamSubsystem()
{
	if (TeamId != INDEX_NONE)
	{
		UDodTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UDodTeamSubsystem>();
		if (ensure(TeamSubsystem))
		{
			RegisterWithTeamSubsystem(TeamSubsystem);
		}
	}
}

void ADodTeamInfoBase::OnRep_TeamId()
{
	TryRegisterWithTeamSubsystem();
}
