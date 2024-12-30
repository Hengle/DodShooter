#include "GameMode/GameState/MenuGameState.h"
#include "NativeGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "ControlFlowManager.h"
#include "PrimaryGameLayout.h"

namespace FrontendTags
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
}

void AMenuGameState::BeginPlay()
{
	Super::BeginPlay();

	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("FrontendFlow"))
	                     .QueueStep(
		                     TEXT("Try Join Requested Session"), this, &ThisClass::FlowStep_TryJoinRequestedSession)
	                     .QueueStep(TEXT("Try Show Main Screen"), this, &ThisClass::FlowStep_TryShowMainScreen);

	Flow.ExecuteFlow();

	FrontEndFlow = Flow.AsShared();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController->SetShowMouseCursor(true);

		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}

bool AMenuGameState::ShouldShowLoadingScreen() const
{
	return bShouldShowLoadingScreen;
}

void AMenuGameState::FlowStep_TryJoinRequestedSession(FControlFlowNodeRef SubFlow)
{
	SubFlow->ContinueFlow();
}

void AMenuGameState::FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow)
{
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this))
	{
		constexpr bool bSuspendInputUntilComplete = true;
		RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(
			FrontendTags::TAG_UI_LAYER_MENU, bSuspendInputUntilComplete, MenuClass,
			[this, SubFlow](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen)
			{
				switch (State)
				{
				case EAsyncWidgetLayerState::AfterPush:
					bShouldShowLoadingScreen = false;
					SubFlow->ContinueFlow();
					return;
				case EAsyncWidgetLayerState::Canceled:
					bShouldShowLoadingScreen = false;
					SubFlow->ContinueFlow();
					break;
				default: break;
				}
			});
	}
}
