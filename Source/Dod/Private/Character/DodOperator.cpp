#include "Character/DodOperator.h"

#include "Character/Comp/DodOperatorComponent.h"

ADodOperator::ADodOperator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OperatorComponent = CreateDefaultSubobject<UDodOperatorComponent>(TEXT("OperatorComponent"));

	HeadMesh->SetupAttachment(GetMesh(), TEXT("j_spine4"));
}

void ADodOperator::BeginPlay()
{
	Super::BeginPlay();
	
}