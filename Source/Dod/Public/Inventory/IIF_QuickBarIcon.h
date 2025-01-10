#pragma once

#include "CoreMinimal.h"
#include "DodInventoryItemDefinition.h"
#include "IIF_QuickBarIcon.generated.h"

UCLASS()
class DOD_API UIIF_QuickBarIcon : public UDodInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush Brush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush AmmoBrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayNameWhenEquipped;
};
