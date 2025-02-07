#include "GameMode/GameState/MenuGameState.h"
#include "NativeGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "ControlFlowManager.h"
#include "PrimaryGameLayout.h"

void AMenuGameState::BeginPlay()
{
	Super::BeginPlay();

	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("FrontendFlow"))
	                     .QueueStep(
		                     TEXT("Try Join Requested Session"), this, &ThisClass::FlowStep_TryJoinRequestedSession)
	                     .QueueStep(TEXT("Try Show Main Screen"), this, &ThisClass::FlowStep_TryShowMainScreen);

	Flow.ExecuteFlow();

	FrontEndFlow = Flow.AsShared();
}

bool AMenuGameState::ShouldShowLoadingScreen(FString& OutReason) const
{
	if (bShouldShowLoadingScreen)
	{
		OutReason = TEXT("Frontend Flow Pending...");

		if (FrontEndFlow.IsValid())
		{
			const TOptional<FString> StepDebugName = FrontEndFlow->GetCurrentStepDebugName();
			if (StepDebugName.IsSet())
			{
				OutReason = StepDebugName.GetValue();
			}
		}

		return true;
	}

	return false;
}

void AMenuGameState::FlowStep_TryJoinRequestedSession(FControlFlowNodeRef SubFlow)
{
	SubFlow->ContinueFlow();
}

void AMenuGameState::FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow)
{
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this))
	{
	}
}
