#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Performance/DodPerformanceStatTypes.h"
#include "DodPerfStatWidgetBase.generated.h"

class UDodPerformanceStatSubsystem;

UCLASS()
class DOD_API UDodPerfStatWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	EDodDisplayablePerformanceStat GetStatToDisplay() const
	{
		return StatToDisplay;
	}

	UFUNCTION(BlueprintPure)
	double FetchStatValue();

protected:
	UPROPERTY(Transient)
	TObjectPtr<UDodPerformanceStatSubsystem> CachedStatSubsystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Display)
	EDodDisplayablePerformanceStat StatToDisplay;
};
