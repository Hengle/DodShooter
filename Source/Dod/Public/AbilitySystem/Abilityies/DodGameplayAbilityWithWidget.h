#pragma once

#include "CoreMinimal.h"
#include "DodGameplayAbility.h"
#include "DodGameplayAbilityWithWidget.generated.h"

struct FUIExtensionHandle;

UCLASS()
class DOD_API UDodGameplayAbilityWithWidget : public UDodGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnAbilityAdded() override;
	virtual void OnAbilityRemoved() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Abilities")
	TArray<TSoftClassPtr<UUserWidget>> WidgetClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Abilities")
	TArray<FGameplayTag> WidgetExtensionPointTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dod|Abilities")
	TArray<FUIExtensionHandle> WidgetExtensionHandles;
};
