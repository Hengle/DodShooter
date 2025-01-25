#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularGameState.h"
#include "DodGameState.generated.h"

class UDodPlayerSpawningManagerComponent;
class UDodAbilitySystemComponent;
class UDodActionSet;

UCLASS()
class DOD_API ADodGameState : public AModularGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADodGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin AActor interface
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	//~ End of AActor interface

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface
	
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<UDodActionSet>> ActionSets;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Dod|GameState")
	UDodPlayerSpawningManagerComponent* PlayerSpawningManager;
	
	UPROPERTY(VisibleAnywhere, Category = "Dod|GameState")
	TObjectPtr<UDodAbilitySystemComponent> AbilitySystemComponent;
};

