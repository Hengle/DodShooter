#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DodAbilitySet.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "DodEquipmentManagerComponent.generated.h"

class UDodAbilitySystemComponent;
struct FDodEquipmentList;
class UDodEquipmentManagerComponent;
class UDodEquipmentDefinition;
class UDodEquipmentInstance;

USTRUCT(BlueprintType)
struct FDodAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

private:
	friend FDodEquipmentList;
	friend UDodEquipmentManagerComponent;

	UPROPERTY()
	TSubclassOf<UDodEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UDodEquipmentInstance> Instance = nullptr;

	UPROPERTY(NotReplicated)
	FDodAbilitySet_GrantedHandles GrantedHandles;
};

USTRUCT(BlueprintType)
struct FDodEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FDodEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FDodEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	UDodEquipmentInstance* AddEntry(TSubclassOf<UDodEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UDodEquipmentInstance* Instance);

private:
	UDodAbilitySystemComponent* GetAbilitySystemComponent() const;

	UPROPERTY()
	TArray<FDodAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

class UDodEquipmentInstance;
class UDodEquipmentDefinition;

UCLASS(BlueprintType, Const)
class DOD_API UDodEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UDodEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UDodEquipmentInstance* EquipItem(TSubclassOf<UDodEquipmentDefinition> EquipmentClass);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UDodEquipmentInstance* ItemInstance);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	FDodEquipmentList EquipmentList;
};
