#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "AbilitySystemInterface.h"
#include "System/GameplayTagStack.h"
#include "Team/DodTeamAgentInterface.h"
#include "DodPlayerState.generated.h"

class UDodAbilitySet;
struct FGameplayTag;
class UDodAbilitySystemComponent;

UCLASS()
class DOD_API ADodPlayerState : public AModularPlayerState, public IAbilitySystemInterface,
                                public IDodTeamAgentInterface
{
	GENERATED_BODY()

public:
	ADodPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Dod|PlayerState")
	ADodPlayerState* GetDodPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Dod|PlayerState")
	UDodAbilitySystemComponent* GetDodAbilitySystemComponent() const { return AbilitySystemComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//~ Begin AActor interface
	virtual void PostInitializeComponents() override;
	//~ End of AActor interface

	//~ Begin IDodTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnDodTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~ End of IDodTeamAgentInterface interface

	UFUNCTION(BlueprintCallable)
	int32 GetTeamId() const
	{
		return GenericTeamIdToInteger(MyTeamID);
	}

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);
	UFUNCTION(BlueprintCallable, Category=Teams)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;
	UFUNCTION(BlueprintCallable, Category=Teams)
	bool HasStatTag(FGameplayTag Tag) const;

private:
	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);

	UFUNCTION()
	void OnRep_MySquadID();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Dod|PlayerState")
	TObjectPtr<UDodAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category="Attributes", meta=(AssetBundles="Client,Server"))
	TArray<TSoftObjectPtr<const UDodAbilitySet>> GrantedAbilitySets;

	UPROPERTY()
	TObjectPtr<const class UDodHealthSet> HealthSet;
	UPROPERTY()
	TObjectPtr<const class UDodCombatSet> CombatSet;

private:
	UPROPERTY()
	FOnDodTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY(ReplicatedUsing=OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY(ReplicatedUsing=OnRep_MySquadID)
	int32 MySquadID;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;
};
