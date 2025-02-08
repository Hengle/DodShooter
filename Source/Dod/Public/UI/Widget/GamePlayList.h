#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayList.generated.h"

class UGameplayTile;
class UDodUserFacingExperienceDefinition;
class UHorizontalBox;

UCLASS()
class DOD_API UGamePlayList : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExperienceSelected, UDodUserFacingExperienceDefinition*, ExpDef);

	UPROPERTY(BlueprintAssignable)
	FOnExperienceSelected OnExperienceSelected;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExperienceLoaded);

	UPROPERTY(BlueprintAssignable)
	FOnExperienceLoaded OnExperienceLoaded;

protected:
	UFUNCTION()
	void OnTileClicked(UDodUserFacingExperienceDefinition* ExpDef);

	UFUNCTION()
	void LoadExpAsset();

	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* ExperienceRowBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Experience")
	TSubclassOf<UGameplayTile> ExperienceTile;
	UPROPERTY()
	TArray<UGameplayTile*> ExperienceWidgets;

	TArray<UDodUserFacingExperienceDefinition*> FacingExperienceList;
	TArray<FPrimaryAssetId> FacingExpPrimaryAssetIdList;
};
