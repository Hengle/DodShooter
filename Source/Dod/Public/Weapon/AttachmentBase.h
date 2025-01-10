#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttachmentBase.generated.h"

UCLASS()
class DOD_API AAttachmentBase : public AActor
{
	GENERATED_BODY()

public:
	AAttachmentBase();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	TObjectPtr<USkeletalMeshComponent> VM_Att;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Weapon")
	TObjectPtr<USkeletalMeshComponent> WM_Att;
};
