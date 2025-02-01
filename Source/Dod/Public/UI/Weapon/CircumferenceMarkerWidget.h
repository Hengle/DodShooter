#pragma once

#include "CoreMinimal.h"
#include "SCircumferenceMarkerWidget.h"
#include "Components/Widget.h"
#include "CircumferenceMarkerWidget.generated.h"

struct FCircumferenceMarkerEntry;

UCLASS()
class DOD_API UCircumferenceMarkerWidget : public UWidget
{
	GENERATED_BODY()

public:
	UCircumferenceMarkerWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin UWidget interface
	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End of UWidget interface

	//~ Begin UVisual interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End of UVisual interface

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetRadius(float InRadius);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TArray<FCircumferenceMarkerEntry> MarkerList;

	/** The radius of the circle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=(ClampMin=0.0))
	float Radius = 48.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush MarkerImage;

protected:
	TSharedPtr<SCircumferenceMarkerWidget> MyMarkerWidget;
};
