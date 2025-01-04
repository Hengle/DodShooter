#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DodActionSet.generated.h"

class UGameFeatureAction;

UCLASS()
class DOD_API UDodActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category="Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
