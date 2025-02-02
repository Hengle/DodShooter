#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "DodBotCreationComponent.generated.h"


class AAIController;

UCLASS(Blueprintable, Abstract)
class DOD_API UDodBotCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UDodBotCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

public:
#if WITH_SERVER_CODE
	FString CreateBotName(int32 PlayerIndex);
#endif

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category=Gameplay)
	void ServerCreateBots();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Gameplay)
	virtual void SpawnOneBot();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Gameplay)
	TSubclassOf<AAIController> BotControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Gameplay)
	int32 NumBotsToCreate{0};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Gameplay)
	TArray<FString> RandomBotNames;

	TArray<FString> RemainingBotNames;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AAIController>> SpawnedBotList;
};
