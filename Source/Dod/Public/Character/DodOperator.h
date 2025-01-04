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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dod|Character")
	TObjectPtr<UDodOperatorComponent> OperatorComponent;
};
