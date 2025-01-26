#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DodGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDodGameModePlayerInitialized, AGameModeBase*, AController*);

UCLASS()
class DOD_API ADodGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADodGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void GenericPlayerInitialization(AController* C) override;
	//~ End of AGameModeBase interface

	FOnDodGameModePlayerInitialized OnGameModePlayerInitialized;
};
