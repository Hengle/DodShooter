#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "DodInventoryManagerComponent.generated.h"

class UDodInventoryManagerComponent;
struct FDodInventoryList;
class UDodInventoryItemInstance;
class UDodInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FDodInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

private:
	friend FDodInventoryList;
	friend UDodInventoryManagerComponent;

	UPROPERTY()
	TObjectPtr<UDodInventoryItemInstance> Instance{nullptr};

	UPROPERTY()
	int32 StackCount{0};

	UPROPERTY(NotReplicated)
	int32 LastObservedCount{INDEX_NONE};
};

USTRUCT(BlueprintType)
struct FDodInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FDodInventoryList() : OwnerComponent(nullptr)
	{
	}

	FDodInventoryList(UActorComponent* InOwnerComponent): OwnerComponent(InOwnerComponent)
	{
	}

	UDodInventoryItemInstance* AddEntry(TSubclassOf<UDodInventoryItemDefinition> ItemClass, int32 StackCount);

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FDodInventoryEntry, FDodInventoryList>(
			Entries, DeltaParms, *this);
	}

protected:
	friend UDodInventoryManagerComponent;

	UPROPERTY()
	TArray<FDodInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FDodInventoryList> : public TStructOpsTypeTraitsBase2<FDodInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

UCLASS(BlueprintType)
class DOD_API UDodInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDodInventoryManagerComponent(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UDodInventoryItemInstance* AddItemDefinition(TSubclassOf<UDodInventoryItemDefinition> ItemDef,
	                                             int32 StackCount = 1);

protected:
	//~ Begin UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	                                 FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~ End of UObject interface
	UPROPERTY(Replicated)
	FDodInventoryList InventoryList;
};
