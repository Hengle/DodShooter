#include "AbilitySystem/Abilityies/DodGameplayAbilityWithWidget.h"

#include "UIExtensionSubsystem.h"
#include "Blueprint/UserWidget.h"

void UDodGameplayAbilityWithWidget::OnAbilityAdded()
{
	Super::OnAbilityAdded();

	for (int32 Index = 0; Index < WidgetClasses.Num(); Index++)
	{
		TSubclassOf<UUserWidget> WidgetClass = WidgetClasses[Index].LoadSynchronous();
		UUIExtensionSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		if (IsValid(WidgetClass) && IsValid(ExtensionSubsystem) && WidgetExtensionPointTags.IsValidIndex(Index))
		{
			WidgetExtensionHandles.Add(
				ExtensionSubsystem->RegisterExtensionAsWidgetForContext(
					WidgetExtensionPointTags[Index],
					GetOwningActorFromActorInfo(),
					WidgetClass,
					-1)
			);
		}
	}
}

void UDodGameplayAbilityWithWidget::OnAbilityRemoved()
{
	Super::OnAbilityRemoved();

	for (FUIExtensionHandle& Handle : WidgetExtensionHandles)
	{
		Handle.Unregister();
	}
	WidgetExtensionHandles.Empty();
}
