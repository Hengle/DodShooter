#pragma once

#include "CoreMinimal.h"
#include "DodTeamInfoBase.h"
#include "DodTeamPublicInfo.generated.h"

class UDodTeamDisplayAsset;

UCLASS()
class DOD_API ADodTeamPublicInfo : public ADodTeamInfoBase
{
	GENERATED_BODY()

public:
	ADodTeamPublicInfo();

	UDodTeamDisplayAsset* GetTeamDisplayAsset() const { return TeamDisplayAsset; }

	void SetTeamDisplayAsset(TObjectPtr<UDodTeamDisplayAsset> NewDisplayAsset);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_TeamDisplayAsset();

	UPROPERTY(ReplicatedUsing=OnRep_TeamDisplayAsset)
	TObjectPtr<UDodTeamDisplayAsset> TeamDisplayAsset;
};
