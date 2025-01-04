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

protected:
	UPROPERTY()
	TArray<FDodInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
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
	UPROPERTY(Replicated)
	FDodInventoryList InventoryList;
};
