#include "GameFeatures/GFA_AddWidget.h"

#include "CommonActivatableWidget.h"
#include "CommonUIExtensions.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Misc/DataValidation.h"
#include "UI/DodHUD.h"

#define LOCTEXT_NAMESPACE "DodGameFeatures"

void UGFA_AddWidget::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* ActiveData = ContextData.Find(Context);
	if ensure(ActiveData)
	{
		Reset(*ActiveData);
	}
}
#if WITH_EDITORONLY_DATA
void UGFA_AddWidget::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	for (const FDodHUDElementEntry& Entry : Widgets)
	{
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient,
		                               Entry.WidgetClass.ToSoftObjectPath().GetAssetPath());
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UGFA_AddWidget::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context),
	                                                            EDataValidationResult::Valid);

	{
		int32 EntryIndex = 0;
		for (const FDodHUDLayoutRequest& Entry : Layout)
		{
			if (Entry.LayoutClass.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(
					LOCTEXT("LayoutHasNullClass", "Null WidgetClass at index {0} in Layout"),
					FText::AsNumber(EntryIndex)));
			}

			if (!Entry.LayerID.IsValid())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(
					LOCTEXT("LayoutHasNoTag", "LayerID is not set at index {0} in Widgets"),
					FText::AsNumber(EntryIndex)));
			}

			++EntryIndex;
		}
	}

	{
		int32 EntryIndex = 0;
		for (const FDodHUDElementEntry& Entry : Widgets)
		{
			if (Entry.WidgetClass.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(
					LOCTEXT("EntryHasNullClass", "Null WidgetClass at index {0} in Widgets"),
					FText::AsNumber(EntryIndex)));
			}

			if (!Entry.SlotID.IsValid())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(
					LOCTEXT("EntryHasNoTag", "SlotID is not set at index {0} in Widgets"),
					FText::AsNumber(EntryIndex)));
			}
			++EntryIndex;
		}
	}

	return Result;
}
#endif

void UGFA_AddWidget::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if (GameInstance && World && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<
			UGameFrameworkComponentManager>(GameInstance))
		{
			TSoftClassPtr<AActor> HUDActorClass = ADodHUD::StaticClass();

			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle =
				ComponentManager->AddExtensionHandler(HUDActorClass,
				                                      UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
					                                      this, &ThisClass::HandleActorExtension, ChangeContext));
			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
		}
	}
}

void UGFA_AddWidget::Reset(FPerContextData& ActiveData)
{
	ActiveData.ComponentRequests.Empty();

	for (TPair<FObjectKey, FPerActorData>& Pair : ActiveData.ActorData)
	{
		for (FUIExtensionHandle& Handle : Pair.Value.ExtensionHandles)
		{
			Handle.Unregister();
		}
	}
	ActiveData.ActorData.Empty();
}

void UGFA_AddWidget::HandleActorExtension(AActor* Actor, FName EventName,
                                                 FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved ||
		EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
	{
		RemoveWidgets(Actor, ActiveData);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded)
	{
		AddWidgets(Actor, ActiveData);
	}
}

void UGFA_AddWidget::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ADodHUD* HUD = CastChecked<ADodHUD>(Actor);

	if (!HUD->GetOwningPlayerController())
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		FPerActorData& ActorData = ActiveData.ActorData.FindOrAdd(HUD);

		for (const FDodHUDLayoutRequest& Entry : Layout)
		{
			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.LoadSynchronous())
			{
				ActorData.LayoutsAdded.Add(
					UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcreteWidgetClass));
			}
		}

		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for (const FDodHUDElementEntry& Entry : Widgets)
		{
			ActorData.ExtensionHandles.Add(
				ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer,
				                                                        Entry.WidgetClass.LoadSynchronous(), -1));
		}
	}
}

void UGFA_AddWidget::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ADodHUD* HUD = CastChecked<ADodHUD>(Actor);

	FPerActorData* ActorData = ActiveData.ActorData.Find(HUD);

	if (ActorData)
	{
		for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout : ActorData->LayoutsAdded)
		{
			if (AddedLayout.IsValid())
			{
				AddedLayout->DeactivateWidget();
			}
		}

		for (FUIExtensionHandle& Handle : ActorData->ExtensionHandles)
		{
			Handle.Unregister();
		}
		ActiveData.ActorData.Remove(HUD);
	}
}

#undef LOCTEXT_NAMESPACE
