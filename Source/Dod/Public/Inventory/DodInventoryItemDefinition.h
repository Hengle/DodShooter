#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DodInventoryItemDefinition.generated.h"

class UDodInventoryItemInstance;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class DOD_API UDodInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UDodInventoryItemInstance* Instance) const
	{
	}
};

UCLASS(Blueprintable, Const, Abstract)
class UDodInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	const UDodInventoryItemFragment* FindFragmentByClass(TSubclassOf<UDodInventoryItemFragment> FragmentClass) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UDodInventoryItemFragment>> Fragments;
};
