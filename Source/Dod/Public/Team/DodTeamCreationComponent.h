// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "DodTeamCreationComponent.generated.h"

class ADodPlayerState;
class ADodTeamPublicInfo;
class ADodTeamPrivateInfo;
class UDodTeamDisplayAsset;

UCLASS(Blueprintable)
class DOD_API UDodTeamCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UDodTeamCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
#if WITH_SERVER_CODE
	virtual void ServerCreateTeams();
	virtual void ServerAssignPlayersToTeams();

	virtual void ServerChooseTeamForPlayer(ADodPlayerState* PS);

	void OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer);
	void ServerCreateTeam(int32 TeamId, UDodTeamDisplayAsset* DisplayAsset);

	int32 GetLeastPopulatedTeamID() const;
#endif

	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TMap<uint8, TObjectPtr<UDodTeamDisplayAsset>> TeamsToCreate;

	UPROPERTY(EditDefaultsOnly, Category=Teams)
	TSubclassOf<ADodTeamPublicInfo> PublicTeamInfoClass;

	UPROPERTY(EditDefaultsOnly, Category=Teams)
	TSubclassOf<ADodTeamPrivateInfo> PrivateTeamInfoClass;
};
