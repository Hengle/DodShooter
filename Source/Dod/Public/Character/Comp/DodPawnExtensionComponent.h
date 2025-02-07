#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "DodPawnExtensionComponent.generated.h"

class UDodPawnData;
class UDodAbilitySet;
class UDodAbilitySystemComponent;
class UDodCameraMode;
class UDodInputConfig;

UCLASS()
class DOD_API UDodPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UDodPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	static const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	                                FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	                                   FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

	void HandleControllerChanged();
	void HandlePlayerStateReplicated();
	void SetupPlayerInputComponent();

	/* 返回Actor是否有该组件 */
	UFUNCTION(BlueprintPure, Category = "Dod|Pawn")
	static UDodPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<UDodPawnExtensionComponent>() : nullptr;
	}

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UDodPawnData* InPawnData);

	UFUNCTION(BlueprintPure, Category = "Dod|Pawn")
	UDodAbilitySystemComponent* GetDodAbilitySystemComponent() const { return AbilitySystemComponent; }

	void InitializeAbilitySystem(UDodAbilitySystemComponent* InAsc, AActor* InOwner);
	void UninitializeAbilitySystem();

	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_PawnData();

	FSimpleMulticastDelegate OnAbilitySystemInitialized;
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;

	UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category = "Dod|Pawn")
	TObjectPtr<const UDodPawnData> PawnData;

	UPROPERTY(Transient)
	TObjectPtr<UDodAbilitySystemComponent> AbilitySystemComponent{nullptr};
};
