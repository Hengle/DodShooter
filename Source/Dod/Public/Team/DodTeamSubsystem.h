#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DodTeamSubsystem.generated.h"

class ADodPlayerState;
class UDodTeamDisplayAsset;
class ADodTeamPrivateInfo;
struct FGameplayTag;
class ADodTeamInfoBase;
class ADodTeamPublicInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDodTeamDisplayAssetChangedDelegate, const UDodTeamDisplayAsset*,
                                            DisplayAsset);

USTRUCT()
struct FDodTeamTrackingInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ADodTeamPublicInfo> PublicInfo = nullptr;

	UPROPERTY()
	TObjectPtr<ADodTeamPrivateInfo> PrivateInfo = nullptr;

	UPROPERTY()
	TObjectPtr<UDodTeamDisplayAsset> DisplayAsset{nullptr};

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

	int32 FindTeamFromObject(const UObject* TestObject) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Teams, meta=(Keywords="Get"))
	void FindTeamFromActor(const UObject* TestActor, bool& bIsPartOfTeam, int32& TeamId) const;
	
	const ADodPlayerState* FindPlayerStateFromActor(const AActor* PossibleTeamActor) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Teams, meta=(ExpandEnumAsExecs=ReturnValue))
	EDodTeamComparison CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const;
	EDodTeamComparison CompareTeams(const UObject* A, const UObject* B) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void AddTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount);
	
	UFUNCTION(BlueprintCallable, Category=Teams)
	int32 GetTeamTagStackCount(int32 TeamId, FGameplayTag Tag) const;

private:
	UPROPERTY()
	TMap<int32, FDodTeamTrackingInfo> TeamMap;
};
