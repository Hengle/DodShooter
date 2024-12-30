#pragma once

#include "CoreMinimal.h"
#include "GameUIPolicy.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIManagerSubsystem.generated.h"

class UCommonLocalPlayer;

UCLASS(Abstract, config = Game)
class COMMONGAME_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGameUIManagerSubsystem();

	//~ Subsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End of Subsystem interface

	const UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

	virtual void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

private:
	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy{nullptr};
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
};
