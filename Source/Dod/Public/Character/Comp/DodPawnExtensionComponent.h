#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "DodPawnExtensionComponent.generated.h"

class UDodAbilitySet;
class UDodAbilitySystemComponent;
class UDodCameraMode;
class UDodInputConfig;

USTRUCT(BlueprintType, Meta = (DisplayName = "Dod Pawn Data", ShortTooltip = "Define a Pawn."))
struct FDodPawnData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Abilities")
	TArray<TObjectPtr<UDodAbilitySet>> AbilitySets;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Abilities")
	TObjectPtr<UDodAbilityTagRelationshipMapping> TagRelationshipMapping;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Input")
	TObjectPtr<UDodInputConfig> InputConfig;
};

UCLASS()
class DOD_API UDodPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UDodPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	static const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	                                FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	                                   FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End of IGameFrameworkInitStateInterface interface

	void HandleControllerChanged();
	void HandlePlayerStateReplicated();
	void SetupPlayerInputComponent();

	/* 返回Actor是否有该组件 */
	UFUNCTION(BlueprintPure, Category = "Dod|Pawn")
	static UDodPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<UDodPawnExtensionComponent>() : nullptr;
	}

	FORCEINLINE const FDodPawnData& GetPawnData() const { return PawnData; }

	UFUNCTION(BlueprintPure, Category = "Dod|Pawn")
	UDodAbilitySystemComponent* GetDodAbilitySystemComponent() const { return AbilitySystemComponent; }

	void InitializeAbilitySystem(UDodAbilitySystemComponent* InAsc, AActor* InOwner);
	void UninitializeAbilitySystem();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dod|Pawn")
	FDodPawnData PawnData;

	UPROPERTY(Transient)
	TObjectPtr<UDodAbilitySystemComponent> AbilitySystemComponent{nullptr};
};
