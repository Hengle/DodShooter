#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DodTeamSubsystem.generated.h"

class ADodTeamInfoBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDodTeamDisplayAssetChangedDelegate, const UDodTeamDisplayAsset*,
                                            DisplayAsset);

USTRUCT()
struct FDodTeamTrackingInfo
{
	GENERATED_BODY()

	/*UPROPERTY()
	TObjectPtr<ADodTeamPublicInfo> PublicInfo{nullptr};

	UPROPERTY()
	TObjectPtr<UDodTeamDisplayAsset> DisplayAsset{nullptr};*/

	UPROPERTY()
	FOnDodTeamDisplayAssetChangedDelegate OnTeamDisplayAssetChanged;

	void SetTeamInfo(ADodTeamInfoBase* Info);
	void RemoveTeamInfo(ADodTeamInfoBase* Info);
};

UENUM(BlueprintType)
enum class EDodTeamComparison : uint8
{
	OnSameTeam,
	DifferentTeams,
	InvalidArgument
};

UCLASS()
class DOD_API UDodTeamSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	bool RegisterTeamInfo(ADodTeamInfoBase* TeamInfo);
	bool UnregisterTeamInfo(ADodTeamInfoBase* TeamInfo);

	bool ChangeTeamForActor(AActor* ActorToChange, int32 NewTeamIndex);

private:
	UPROPERTY()
	TMap<int32, FDodTeamTrackingInfo> TeamMap;
};
