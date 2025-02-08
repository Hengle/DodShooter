#pragma once

#include "CoreMinimal.h"
#include "UI/Foundation/DodButtonBase.h"
#include "GameplayTileButton.generated.h"

class UImage;
class UCommonTextBlock;

UCLASS()
class DOD_API UGameplayTileButton : public UDodButtonBase
{
	GENERATED_BODY()

public:
	void UpdateTileText(FText InTitle, FText InMapTitle);
	void UpdateTileArt(UTexture* TileArt);

protected:
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* ExpTitle;
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* MapTitle;
	UPROPERTY(meta = (BindWidget))
	UImage* KeyArt;
};
