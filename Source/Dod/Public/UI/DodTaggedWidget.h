#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "DodTaggedWidget.generated.h"

class UAbilitySystemComponent;
/* UDodTaggedWidget
 * 通过Tag控制Widget的显示状态
 */
UCLASS(Abstract, Blueprintable)
class DOD_API UDodTaggedWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	//~ Begin UWidget interface
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	//~ End of UWidget interface

	//~ Begin UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~ End of UUserWidget interface
protected:
	UFUNCTION()
	void OnWatchedTagChanged(const FGameplayTag Tag, int32 NewCount);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	FGameplayTag HiddenByTag;

	UPROPERTY(EditAnywhere, Category = "HUD")
	ESlateVisibility ShownVisibility = ESlateVisibility::Visible;

	UPROPERTY(EditAnywhere, Category = "HUD")
	ESlateVisibility HiddenVisibility = ESlateVisibility::Collapsed;

	bool bWantsToBeVisible{true};

private:
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	FDelegateHandle TagDelegateHandle;
};
