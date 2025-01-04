#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayAbilitySpecHandle.h"
#include "ActiveGameplayEffectHandle.h"
#include "DodGlobalAbilitySystem.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UDodAbilitySystemComponent;

USTRUCT()
struct FGlobalAppliedAbilityList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UDodAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UDodAbilitySystemComponent* ASC);
	void RemoveFromASC(UDodAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

USTRUCT()
struct FGlobalAppliedEffectList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UDodAbilitySystemComponent>, FActiveGameplayEffectHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UDodAbilitySystemComponent* ASC);
	void RemoveFromASC(UDodAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

UCLASS()
class DOD_API UDodGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterASC(UDodAbilitySystemComponent* ASC);

	void UnregisterASC(UDodAbilitySystemComponent* ASC);

protected:
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;

	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	UPROPERTY()
	TArray<TObjectPtr<UDodAbilitySystemComponent>> RegisteredASCs;
};
