#pragma once

#include "CoreMinimal.h"
#include "DodCharacter.h"
#include "DodOperator.generated.h"

class UDodOperatorComponent;

UCLASS()
class DOD_API ADodOperator : public ADodCharacter
{
	GENERATED_BODY()

public:
	ADodOperator(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	virtual void OnDeathStarted(AActor* OwningActor) override;

	void Ragdoll();
	void HideEquippedWeapons();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dod|Character")
	TObjectPtr<UDodOperatorComponent> OperatorComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ragdoll")
	float RagdollImpulseStrength{500.f};
};
