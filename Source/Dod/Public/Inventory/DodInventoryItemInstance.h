#pragma once

#include "CoreMinimal.h"
#include "System/GameplayTagStack.h"
#include "UObject/Object.h"
#include "DodInventoryItemInstance.generated.h"

class UDodInventoryItemFragment;
class UDodInventoryItemDefinition;

UCLASS(BlueprintType)
class DOD_API UDodInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	//~ Begin UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~ End of UObject interface

	// 添加Tag和数值
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// 移除Tag的一些数值
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category= Inventory)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// 返回Tag的数值
	UFUNCTION(BlueprintCallable, Category=Inventory)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// 是否拥有Tag
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool HasStatTag(FGameplayTag Tag) const;

	TSubclassOf<UDodInventoryItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}

	void SetItemDef(TSubclassOf<UDodInventoryItemDefinition> InDef);

	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UDodInventoryItemFragment* FindFragmentByClass(TSubclassOf<UDodInventoryItemFragment> FragmentClass) const;

	template <typename T>
	const T* FindFragmentByClass() const
	{
		return Cast<T>(FindFragmentByClass(T::StaticClass()));
	}

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

#if UE_WITH_IRIS
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
	                                          UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
#endif // UE_WITH_IRIS

private:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;
	UPROPERTY(Replicated)
	TSubclassOf<UDodInventoryItemDefinition> ItemDef;
};
