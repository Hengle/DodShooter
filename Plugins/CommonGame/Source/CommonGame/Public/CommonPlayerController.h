#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"

#include "CommonPlayerController.generated.h"

UCLASS()
class COMMONGAME_API ACommonPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ACommonPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin AModularPlayerController
	virtual void ReceivedPlayer() override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	//~ End of AModularPlayerController
};
