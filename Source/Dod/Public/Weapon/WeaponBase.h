#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class DOD_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Dod|Weapon")
	USkeletalMeshComponent* VM_Receiver;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Dod|Weapon")
	USkeletalMeshComponent* WM_Receiver;
};
