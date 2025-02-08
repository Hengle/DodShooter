#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTile.generated.h"

class UGameplayTileButton;
class UDodUserFacingExperienceDefinition;

UCLASS()
class DOD_API UGameplayTile : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetUserFacingExperience(UDodUserFacingExperienceDefinition* NewExp);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClicked, UDodUserFacingExperienceDefinition*, ExpDef);

	UPROPERTY(BlueprintAssignable)
	FOnClicked OnClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	UGameplayTileButton* GameplayTileButton{nullptr};

private:
	UPROPERTY(Transient)
	UDodUserFacingExperienceDefinition* CurrentExperience;
};
