#include "Character/DodOperator.h"

#include "Character/Comp/DodOperatorComponent.h"

ADodOperator::ADodOperator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OperatorComponent = CreateDefaultSubobject<UDodOperatorComponent>(TEXT("OperatorComponent"));
}

void ADodOperator::BeginPlay()
{
	Super::BeginPlay();
	
}