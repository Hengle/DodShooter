#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "AbilitySystemInterface.h"
#include "DodPlayerState.generated.h"

class UDodAbilitySystemComponent;

UCLASS()
class DOD_API ADodPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADodPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Dod|PlayerState")
	ADodPlayerState* GetDodPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Dod|PlayerState")
	UDodAbilitySystemComponent* GetDodAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//~Begin AActor interface
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

protected:
	UPROPERTY(VisibleAnywhere, Category = "Dod|PlayerState")
	TObjectPtr<UDodAbilitySystemComponent> AbilitySystemComponent;
};
