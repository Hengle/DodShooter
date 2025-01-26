#include "Team/DodTeamPublicInfo.h"

#include "Net/UnrealNetwork.h"


ADodTeamPublicInfo::ADodTeamPublicInfo()
{
}

void ADodTeamPublicInfo::SetTeamDisplayAsset(TObjectPtr<UDodTeamDisplayAsset> NewDisplayAsset)
{
	check(HasAuthority());
	check(TeamDisplayAsset == nullptr);

	TeamDisplayAsset = NewDisplayAsset;

	TryRegisterWithTeamSubsystem();
}

void ADodTeamPublicInfo::BeginPlay()
{
	Super::BeginPlay();
}

void ADodTeamPublicInfo::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, TeamDisplayAsset, COND_InitialOnly);
}

void ADodTeamPublicInfo::OnRep_TeamDisplayAsset()
{
	TryRegisterWithTeamSubsystem();
}
