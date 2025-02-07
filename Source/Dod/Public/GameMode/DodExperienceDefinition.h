#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DodExperienceDefinition.generated.h"

class UDodExperienceActionSet;
class UDodPawnData;
class UGameFeatureAction;

UCLASS()
class DOD_API UDodExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	//~UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~End of UObject interface

#if WITH_EDITORONLY_DATA
	//~ Begin UPrimaryDataAsset interface
	virtual void UpdateAssetBundleData() override;
	//~ End UPrimaryDataAsset interface
#endif

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<const UDodPawnData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<UDodExperienceActionSet>> ActionSets;
};
