#pragma once

#include "CoreMinimal.h"
#include "GFA_WorldActionBase.h"
#include "GFA_AddAbilities.generated.h"

class UGameplayAbility;
class UDodAbilitySet;
struct FComponentRequestHandle;
struct FDodAbilitySet_GrantedHandles;
class UAttributeSet;
struct FGameplayAbilitySpecHandle;

USTRUCT(BlueprintType)
struct FDodAbilityGrant
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AssetBundles="Client,Server"))
	TSoftClassPtr<UGameplayAbility> AbilityType;
};

USTRUCT(BlueprintType)
struct FDodAttributeSetGrant
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AssetBundles="Client,Server"))
	TSoftClassPtr<UAttributeSet> AttributeSetType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AssetBundles="Client,Server"))
	TSoftObjectPtr<UDataTable> InitializationData;
};

USTRUCT()
struct FGameFeatureAbilitiesEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSoftClassPtr<AActor> ActorClass;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<FDodAbilityGrant> GrantedAbilities;

	UPROPERTY(EditAnywhere, Category="Attributes")
	TArray<FDodAttributeSetGrant> GrantedAttributes;

	UPROPERTY(EditAnywhere, Category="Attributes", meta=(AssetBundles="Client,Server"))
	TArray<TSoftObjectPtr<const UDodAbilitySet>> GrantedAbilitySets;
};

UCLASS(MinimalAPI, meta = (DisplayName = "Add Abilities"))
class UGFA_AddAbilities : public UGFA_WorldActionBase
{
	GENERATED_BODY()

public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~ End UGameFeatureAction interface

	//~ Begin UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~ End UObject interface
private:
	struct FActorExtensions
	{
		TArray<FGameplayAbilitySpecHandle> Abilities;
		TArray<UAttributeSet*> Attributes;
		TArray<FDodAbilitySet_GrantedHandles> AbilitySetHandles;
	};

	struct FPerContextData
	{
		TMap<AActor*, FActorExtensions> ActiveExtensions;
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
	};

	//~ Begin UGameFeatureAction_WorldActionBase interface
	virtual void AddToWorld(const FWorldContext& WorldContext,
	                        const FGameFeatureStateChangeContext& ChangeContext) override;
	//~ End UGameFeatureAction_WorldActionBase interface

	void Reset(FPerContextData& ActiveData);
	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex,
	                          FGameFeatureStateChangeContext ChangeContext);
	void AddActorAbilities(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry,
	                       FPerContextData& ActiveData);
	void RemoveActorAbilities(AActor* Actor, FPerContextData& ActiveData);

	template <class ComponentType>
	ComponentType* FindComponentForActor(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry,
	                                     FPerContextData& ActiveData)
	{
		return Cast<ComponentType>(
			FindComponentForActor(ComponentType::StaticClass(), Actor, AbilitiesEntry, ActiveData));
	}

	UActorComponent* FindComponentForActor(UClass* ComponentType, AActor* Actor,
	                                       const FGameFeatureAbilitiesEntry& AbilitiesEntry,
	                                       FPerContextData& ActiveData);

public:
	UPROPERTY(EditAnywhere, Category="Abilities", meta=(TitleProperty="ActorClass", ShowOnlyInnerProperties))
	TArray<FGameFeatureAbilitiesEntry> AbilitiesList;

private:
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
};
