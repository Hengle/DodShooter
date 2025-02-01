#pragma once

#include "CoreMinimal.h"
#include "SHitMarkerConfirmationWidget.h"
#include "Components/Widget.h"
#include "HitMarkerConfirmationWidget.generated.h"

struct FGameplayTag;

UCLASS()
class DOD_API UHitMarkerConfirmationWidget : public UWidget
{
	GENERATED_BODY()

public:
	UHitMarkerConfirmationWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin UVisual interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End of UVisual interface
protected:
	//~ Begin UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End of UWidget interface
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=(ClampMin=0.0, ForceUnits=s))
	float HitNotifyDuration = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush PerHitMarkerImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TMap<FGameplayTag, FSlateBrush> PerHitMarkerZoneOverrideImages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateBrush AnyHitsMarkerImage;

private:
	TSharedPtr<SHitMarkerConfirmationWidget> MyMarkerWidget;
};
