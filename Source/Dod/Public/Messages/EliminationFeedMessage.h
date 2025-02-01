#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EliminationFeedMessage.generated.h"

USTRUCT(BlueprintType)
struct FEliminationFeedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FText Attacker;
	UPROPERTY(BlueprintReadWrite)
	FText Victim;
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag TargetChannel;
	UPROPERTY(BlueprintReadWrite)
	APlayerState* TargetPlayer;
	UPROPERTY(BlueprintReadWrite)
	int32 AttackerTeamId;
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer InstigatorTags;
	UPROPERTY(BlueprintReadWrite)
	int32 VictimTeamId;
};
