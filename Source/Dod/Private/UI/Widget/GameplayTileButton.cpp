#include "UI/Widget/GameplayTileButton.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"

void UGameplayTileButton::UpdateTileText(FText InTitle, FText InMapTitle)
{
	ExpTitle->SetText(InTitle);
	MapTitle->SetText(InMapTitle);
}

void UGameplayTileButton::UpdateTileArt(UTexture* TileArt)
{
	FSlateBrush Brush = KeyArt->GetBrush();
	Brush.SetResourceObject(TileArt);
	KeyArt->SetBrush(Brush);
	// KeyArt->GetDynamicMaterial()->SetTextureParameterValue(TEXT("TileArt"),TileArt);
}
