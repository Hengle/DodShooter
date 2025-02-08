#include "UI/Widget/GameplayTile.h"

#include "GameMode/DodUserFacingExperienceDefinition.h"
#include "UI/Widget/GameplayTileButton.h"

void UGameplayTile::NativeConstruct()
{
	Super::NativeConstruct();
	GameplayTileButton->OnClicked().AddLambda([&]()
	{
		OnClicked.Broadcast(CurrentExperience);
	});
}

void UGameplayTile::SetUserFacingExperience(UDodUserFacingExperienceDefinition* NewExp)
{
	CurrentExperience = NewExp;
	if (GameplayTileButton)
	{
		GameplayTileButton->UpdateTileText(CurrentExperience->TileTitle, CurrentExperience->TileSubTitle);
		GameplayTileButton->UpdateTileArt(CurrentExperience->TileIcon);
	}
}
