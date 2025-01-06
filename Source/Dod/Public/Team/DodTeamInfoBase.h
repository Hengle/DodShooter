#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "DodTeamInfoBase.generated.h"

class UDodTeamSubsystem;

UCLASS()
class DOD_API ADodTeamInfoBase : public AInfo
{
	GENERATED_BODY()

public:
	ADodTeamInfoBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	int32 GetTeamId() const { return TeamId; }
	void SetTeamId(int32 NewTeamId);

protected:
	//~ Begin AActor interface
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End of AActor interface
	virtual void RegisterWithTeamSubsystem(UDodTeamSubsystem* Subsystem);
	void TryRegisterWithTeamSubsystem();

private:
	UFUNCTION()
	void OnRep_TeamId();

	UPROPERTY(ReplicatedUsing=OnRep_TeamId)
	int32 TeamId{INDEX_NONE};
};
