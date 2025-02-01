#pragma once

#include "CoreMinimal.h"
#include "SCircumferenceMarkerWidget.generated.h"

USTRUCT(BlueprintType)
struct FCircumferenceMarkerEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ForceUnits=deg))
	float PositionAngle = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ForceUnits=deg))
	float ImageRotationAngle = 0.0f;
};

class SCircumferenceMarkerWidget : public SLeafWidget
{
	SLATE_BEGIN_ARGS(SCircumferenceMarkerWidget)
			: _MarkerBrush(FCoreStyle::Get().GetBrush("Throbber.CircleChunk"))
			  , _Radius(48.0f)
		{
		}

		SLATE_ARGUMENT(const FSlateBrush*, MarkerBrush)
		SLATE_ARGUMENT(TArray<FCircumferenceMarkerEntry>, MarkerList)
		SLATE_ATTRIBUTE(float, Radius)
		SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

	//~ Begin SWidget interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual bool ComputeVolatility() const override;
	//~ End of SWidget interface

	void SetRadius(float NewRadius);
	void SetMarkerList(TArray<FCircumferenceMarkerEntry>& NewMarkerList);

private:
	FSlateRenderTransform GetMarkerRenderTransform(const FCircumferenceMarkerEntry& Marker, const float BaseRadius,
	                                               const float HUDScale) const;

	const FSlateBrush* MarkerBrush{nullptr};
	TArray<FCircumferenceMarkerEntry> MarkerList;
	TAttribute<float> Radius;
	bool bColorAndOpacitySet{false};
	TAttribute<FSlateColor> ColorAndOpacity;
	uint8 bReticleCornerOutsideSpreadRadius : 1{false};
};
