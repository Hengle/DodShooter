#include "UI/Widget/GamePlayList.h"

#include "CommonUserWidget.h"
#include "Components/HorizontalBox.h"
#include "Engine/AssetManager.h"
#include "GameMode/DodUserFacingExperienceDefinition.h"
#include "UI/Widget/GameplayTile.h"

void UGamePlayList::NativeConstruct()
{
	Super::NativeConstruct();

	UAssetManager& AssetManager = UAssetManager::Get();

	ExperienceRowBox->ClearChildren();
	FacingExperienceList.Empty();

	FPrimaryAssetType PrimaryAssetType("DodUserFacingExperienceDefinition");

	AssetManager.GetPrimaryAssetIdList(PrimaryAssetType, FacingExpPrimaryAssetIdList);

	if (FacingExpPrimaryAssetIdList.IsEmpty())
	{
		return;
	}
	FStreamableDelegate OnAssetsLoaded = FStreamableDelegate::CreateUFunction(this,TEXT("LoadExpAsset"));

	AssetManager.LoadPrimaryAssets(FacingExpPrimaryAssetIdList, TArray<FName>(), OnAssetsLoaded);
}

void UGamePlayList::OnTileClicked(UDodUserFacingExperienceDefinition* ExpDef)
{
	OnExperienceSelected.Broadcast(ExpDef);
}

void UGamePlayList::LoadExpAsset()
{
	UAssetManager& AssetManager = UAssetManager::Get();
	for (const FPrimaryAssetId& AssetId : FacingExpPrimaryAssetIdList)
	{
		if (UDodUserFacingExperienceDefinition* ExperienceDefinition =
			Cast<UDodUserFacingExperienceDefinition>(AssetManager.GetPrimaryAssetObject(AssetId)))
		{
			if (ExperienceDefinition->bShowInFrontEnd)
			{
				if (ExperienceDefinition->bIsDefaultExperience)
				{
					FacingExperienceList.Insert(ExperienceDefinition, 0);
				}
				else
				{
					FacingExperienceList.Add(ExperienceDefinition);
				}
			}
		}
	}
	if (ExperienceTile)
	{
		for (auto ExperienceDefinition : FacingExperienceList)
		{
			if (UGameplayTile* Tile = CreateWidget<UGameplayTile>(GetOwningPlayer(), ExperienceTile))
			{
				Tile->SetUserFacingExperience(ExperienceDefinition);
				ExperienceWidgets.Add(Tile);
				Tile->OnClicked.AddDynamic(this, &ThisClass::OnTileClicked);
				ExperienceRowBox->AddChildToHorizontalBox(Tile);
			}
		}
		OnExperienceLoaded.Broadcast();
	}
}
