#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Performance/DodPerformanceStatTypes.h"
#include "DodPerfStatContainerBase.generated.h"

UCLASS(Abstract)
class DOD_API UDodPerfStatContainerBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	//~UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~End of UUserWidget interface

	UFUNCTION(BlueprintCallable)
	void UpdateVisibilityOfChildren();

protected:
	// Are we showing text or graph stats?
	UPROPERTY(EditAnywhere, Category=Display)
	EDodStatDisplayMode StatDisplayModeFilter = EDodStatDisplayMode::TextAndGraph;
};
