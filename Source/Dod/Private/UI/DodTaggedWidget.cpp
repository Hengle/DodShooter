#include "UI/DodTaggedWidget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/PlayerState.h"

void UDodTaggedWidget::SetVisibility(ESlateVisibility InVisibility)
{
#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		Super::SetVisibility(InVisibility);
		return;
	}
#endif

	// Remember what the caller requested; even if we're currently being
	// suppressed by a tag we should respect this call when we're done
	bWantsToBeVisible = ConvertSerializedVisibilityToRuntime(InVisibility).IsVisible();
	if (bWantsToBeVisible)
	{
		ShownVisibility = InVisibility;
	}
	else
	{
		HiddenVisibility = InVisibility;
	}

	const ESlateVisibility DesiredVisibility = bWantsToBeVisible ? ShownVisibility : HiddenVisibility;
	if (GetVisibility() != DesiredVisibility)
	{
		Super::SetVisibility(DesiredVisibility);
	}
}

void UDodTaggedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsDesignTime())
	{
		AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningPlayerState());
		if (AbilitySystemComponent)
		{
			TagDelegateHandle = AbilitySystemComponent->RegisterGameplayTagEvent(
				HiddenByTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnWatchedTagChanged);
		}
		SetVisibility(GetVisibility());
	}
}

void UDodTaggedWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->UnregisterGameplayTagEvent(TagDelegateHandle, HiddenByTag);
	}
}

void UDodTaggedWidget::OnWatchedTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	const bool bHasHiddenTags = NewCount <= 0;

	const ESlateVisibility DesiredVisibility = bWantsToBeVisible && !bHasHiddenTags ? ShownVisibility : HiddenVisibility;
	if (GetVisibility() != DesiredVisibility)
	{
		Super::SetVisibility(DesiredVisibility);
	}
}
