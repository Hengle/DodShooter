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

	//~ FFastArraySerializer
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FDodAppliedEquipmentEntry, FDodEquipmentList>(
			Entries, DeltaParms, *this);
	}
	//~ End of FFastArraySerializer

private:
	friend class UDodEquipmentManagerComponent;

	UDodAbilitySystemComponent* GetAbilitySystemComponent() const;

	UPROPERTY()
	TArray<FDodAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template <>
struct TStructOpsTypeTraits<FDodEquipmentList> : public TStructOpsTypeTraitsBase2<FDodEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
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

	//~ Begin UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	                                 FReplicationFlags* RepFlags) override;
	//~ End of UObject interface

	//~ Begin ActorComponent
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~ End of ActorComponent

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDodEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UDodEquipmentInstance> InstanceType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UDodEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UDodEquipmentInstance> InstanceType) const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	FDodEquipmentList EquipmentList;
};
