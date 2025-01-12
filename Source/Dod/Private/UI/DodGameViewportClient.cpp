#include "UI/DodGameViewportClient.h"
#include "ICommonUIModule.h"
#include "CommonUISettings.h"

namespace GameViewportTags
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Platform_Trait_Input_HardwareCursor, "Platform.Trait.Input.HardwareCursor");
}

UDodGameViewportClient::UDodGameViewportClient(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDodGameViewportClient::Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance,
                                  bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);

	const bool UseHardwareCursor = ICommonUIModule::GetSettings().GetPlatformTraits().HasTag(
		GameViewportTags::TAG_Platform_Trait_Input_HardwareCursor);
	SetUseSoftwareCursorWidgets(!UseHardwareCursor);
}