#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DodInventoryItemInstance.generated.h"

class UDodInventoryItemFragment;
class UDodInventoryItemDefinition;

UCLASS()
class DOD_API UDodInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
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
private:
	UPROPERTY(Replicated)
	TSubclassOf<UDodInventoryItemDefinition> ItemDef;
};
