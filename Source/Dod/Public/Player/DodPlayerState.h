#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "AbilitySystemInterface.h"
#include "System/GameplayTagStack.h"
#include "Team/DodTeamAgentInterface.h"
#include "DodPlayerState.generated.h"

class UDodPawnData;
class UDodExperienceDefinition;
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

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UDodPawnData* InPawnData);

	//~ Begin AActor interface
	virtual void PostInitializeComponents() override;
	//~ End AActor interface

	//~ Begin IDodTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnDodTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~ End IDodTeamAgentInterface interface

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
	void OnExperienceLoaded(const UDodExperienceDefinition* CurrentExperience);

	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);

	UFUNCTION()
	void OnRep_MySquadID();

	UFUNCTION()
	void OnRep_PawnData();

public:
	static const FName NAME_DodAbilityReady;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Dod|PlayerState")
	TObjectPtr<UDodAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const class UDodHealthSet> HealthSet;
	UPROPERTY()
	TObjectPtr<const class UDodStaminaSet> StaminaSet;
	UPROPERTY()
	TObjectPtr<const class UDodCombatSet> CombatSet;

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UDodPawnData> PawnData;

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
