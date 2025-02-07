#pragma once

#include "CoreMinimal.h"
#include "ControlFlowNode.h"
#include "DodGameState.h"
#include "LoadingProcessInterface.h"
#include "CommonActivatableWidget.h"
#include "MenuGameState.generated.h"

UCLASS()
class DOD_API AMenuGameState : public ADodGameState, public ILoadingProcessInterface
{
	GENERATED_BODY()

protected:
	//~ GameState interface
	virtual void BeginPlay() override;
	//~ End GameState interface

	//~ ILoadingProcessInterface interface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~ End ILoadingProcessInterface interface
private:
	void FlowStep_TryJoinRequestedSession(FControlFlowNodeRef SubFlow);
	void FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow);

	TSharedPtr<FControlFlow> FrontEndFlow;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> MenuClass;

private:
	bool bShouldShowLoadingScreen = true;
};
