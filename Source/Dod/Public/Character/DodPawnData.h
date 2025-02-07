#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DodPawnData.generated.h"

class UDodAbilityTagRelationshipMapping;
class UDodCameraMode;
class UDodInputConfig;
class UDodAbilitySet;

UCLASS(BlueprintType, Const, Meta = (DisplayName = "Dod Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class DOD_API UDodPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Abilities")
	TArray<TObjectPtr<UDodAbilitySet>> AbilitySets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Abilities")
	TObjectPtr<UDodAbilityTagRelationshipMapping> TagRelationshipMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Input")
	TObjectPtr<UDodInputConfig> InputConfig;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Camera")
	TSubclassOf<UDodCameraMode> DefaultCameraMode;*/
};
