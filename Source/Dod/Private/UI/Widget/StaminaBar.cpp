#include "UI/Widget/StaminaBar.h"

#include "Character/Comp/DodStaminaComponent.h"
#include "Components/ProgressBar.h"

void UStaminaBar::NativeConstruct()
{
	Super::NativeConstruct();

	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
	OnPossessedPawnChanged(nullptr, GetOwningPlayer()->GetPawn());
	BarFill->SetPercent(1.f);
}

void UStaminaBar::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (UDodStaminaComponent* OldHealth = UDodStaminaComponent::FindStaminaComponent(OldPawn))
	{
		OldHealth->OnStaminaChanged.RemoveDynamic(this, &ThisClass::StaminaChanged);
	}
	if (UDodStaminaComponent* NewHealth = UDodStaminaComponent::FindStaminaComponent(NewPawn))
	{
		NewHealth->OnStaminaChanged.AddDynamic(this, &ThisClass::StaminaChanged);
	}
}

void UStaminaBar::StaminaChanged(UDodStaminaComponent* StaminaComponent, float OldValue, float NewValue,
                                 AActor* Instigator)
{
	BarFill->SetPercent(NewValue / StaminaComponent->GetMaxStamina());
}
