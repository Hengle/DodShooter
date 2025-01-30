#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "DodQuickBarComponent.generated.h"


class UDodEquipmentManagerComponent;
class UDodEquipmentInstance;
class UDodInventoryItemInstance;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class DOD_API UDodQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UDodQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Dod")
	void SetActiveSlotIndex(int32 NewIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	TArray<UDodInventoryItemInstance*> GetSlots() const
	{
		return Slots;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UDodInventoryItemInstance* GetActiveSlotItem() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UDodEquipmentInstance* GetEquippedItem() { return EquippedItem; };

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddItemToSlot(int32 SlotIndex, UDodInventoryItemInstance* Item);

	bool IsValidSlotIndex(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UDodInventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void UnequipItemInSlot();
	void EquipItemInSlot();

	UDodEquipmentManagerComponent* FindEquipmentManager() const;

	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_ActiveSlotIndex();

	UPROPERTY()
	int32 NumSlots{2};

private:
	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TArray<TObjectPtr<UDodInventoryItemInstance>> Slots;

	UPROPERTY()
	TObjectPtr<UDodEquipmentInstance> EquippedItem;

	UPROPERTY(ReplicatedUsing=OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;
};
