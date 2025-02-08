#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "DodPlayerSpawningManagerComponent.generated.h"


class ADodPlayerStart;

UCLASS()
class DOD_API UDodPlayerSpawningManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UDodPlayerSpawningManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin UActorComponent interface
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent interface

	//~ Begin ADodGameMode interface
	AActor* ChoosePlayerStart(AController* Player);
	bool ControllerCanRestart(AController* Player);
	void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation);
	//~ End ADodGameMode interface

	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<ADodPlayerStart*>& PlayerStarts) { return nullptr; }

private:
	APlayerStart* GetFirstRandomUnoccupiedPlayerStart(AController* Controller,
	                                                  const TArray<ADodPlayerStart*>& FoundStartPoints) const;

	void OnLevelAdded(ULevel* InLevel, UWorld* InWorld);
	void HandleOnActorSpawned(AActor* SpawnedActor);

#if WITH_EDITOR
	APlayerStart* FindPlayFromHereStart(AController* Player);
#endif

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<ADodPlayerStart>> CachedPlayerStarts;
};
