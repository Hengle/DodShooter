#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DodInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FDodInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS()
class DOD_API UDodInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UDodInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Dod|Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InInputTag) const;

	UFUNCTION(BlueprintCallable, Category = "Dod|Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InInputTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FDodInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FDodInputAction> AbilityInputActions;
};
