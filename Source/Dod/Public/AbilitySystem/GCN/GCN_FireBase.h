#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Burst.h"
#include "GCN_FireBase.generated.h"

class AWeaponBase;

UCLASS()
class DOD_API UGCN_FireBase : public UGameplayCueNotify_Burst
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

	AWeaponBase* GetWeapon(AActor* InActor) const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<class USoundBase> FireSound;
};
