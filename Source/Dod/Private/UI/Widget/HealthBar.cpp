#include "UI/Widget/HealthBar.h"

#include "Character/Comp/DodHealthComponent.h"
#include "Components/ProgressBar.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
	OnPossessedPawnChanged(nullptr, GetOwningPlayer()->GetPawn());
	BarFill->SetPercent(1.f);
	HealthValue->SetCurrentValue(150.f);
}

void UHealthBar::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (UDodHealthComponent* OldHealth = UDodHealthComponent::FindHealthComponent(OldPawn))
	{
		OldHealth->OnHealthChanged.Clear();
	}
	if (UDodHealthComponent* NewHealth = UDodHealthComponent::FindHealthComponent(NewPawn))
	{
		NewHealth->OnHealthChanged.AddDynamic(this, &ThisClass::HealthChanged);
	}
}

void UHealthBar::HealthChanged(UDodHealthComponent* HealthComponent, float OldValue, float NewValue, AActor* Instigator)
{
	BarFill->SetPercent(NewValue / HealthComponent->GetMaxHealth());
	HealthValue->SetCurrentValue(NewValue);
}
