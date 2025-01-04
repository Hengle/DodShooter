#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DodAbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOD_API UDodAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UDodAbilitySystemComponent();

	//~ Begin UAbilitySystemComponent interface
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~ End of UAbilitySystemComponent interface
	
protected:
	void TryActivateAbilitiesOnSpawn();
};
