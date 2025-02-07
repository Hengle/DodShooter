#include "UI/PerformanceStats/DodPerfStatContainerBase.h"

#include "Blueprint/WidgetTree.h"
#include "Settings/DodSettingsLocal.h"
#include "UI/PerformanceStats/DodPerfStatWidgetBase.h"

void UDodPerfStatContainerBase::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateVisibilityOfChildren();

	UDodSettingsLocal::Get()->OnPerfStatDisplayStateChanged().AddUObject(this, &ThisClass::UpdateVisibilityOfChildren);
}

void UDodPerfStatContainerBase::NativeDestruct()
{
	UDodSettingsLocal::Get()->OnPerfStatDisplayStateChanged().RemoveAll(this);
	
	Super::NativeDestruct();
}

void UDodPerfStatContainerBase::UpdateVisibilityOfChildren()
{
	UDodSettingsLocal* UserSettings = UDodSettingsLocal::Get();

	const bool bShowTextWidgets = (StatDisplayModeFilter == EDodStatDisplayMode::TextOnly) || (StatDisplayModeFilter == EDodStatDisplayMode::TextAndGraph);
	const bool bShowGraphWidgets = (StatDisplayModeFilter == EDodStatDisplayMode::GraphOnly) || (StatDisplayModeFilter == EDodStatDisplayMode::TextAndGraph);
	
	check(WidgetTree);
	WidgetTree->ForEachWidget([&](UWidget* Widget)
	{
		if (UDodPerfStatWidgetBase* TypedWidget = Cast<UDodPerfStatWidgetBase>(Widget))
		{
			const EDodStatDisplayMode SettingMode = UserSettings->GetPerfStatDisplayState(TypedWidget->GetStatToDisplay());

			bool bShowWidget = false;
			switch (SettingMode)
			{
			case EDodStatDisplayMode::Hidden:
				bShowWidget = false;
				break;
			case EDodStatDisplayMode::TextOnly:
				bShowWidget = bShowTextWidgets;
				break;
			case EDodStatDisplayMode::GraphOnly:
				bShowWidget = bShowGraphWidgets;
				break;
			case EDodStatDisplayMode::TextAndGraph:
				bShowWidget = bShowTextWidgets || bShowGraphWidgets;
				break;
			}

			TypedWidget->SetVisibility(bShowWidget ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
	});
}
