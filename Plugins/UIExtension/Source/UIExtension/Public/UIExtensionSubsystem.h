#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIExtensionSubsystem.generated.h"

struct FUIExtensionRequest;
class UUIExtensionSubsystem;

UENUM(BlueprintType)
enum class EUIExtensionPointMatch : uint8
{
	// 精准匹配，不匹配子级Tag
	ExactMatch,
	// 部分匹配，匹配自身包括子级Tag
	PartialMatch
};

UENUM(BlueprintType)
enum class EUIExtensionAction : uint8
{
	Added,
	Removed
};

DECLARE_DELEGATE_TwoParams(FExtendExtensionPointDelegate, EUIExtensionAction Action,
                           const FUIExtensionRequest& Request);

struct FUIExtension : TSharedFromThis<FUIExtension>
{
	FGameplayTag ExtensionPointTag;
	int32 Priority = INDEX_NONE;
	TWeakObjectPtr<UObject> ContextObject;
	TObjectPtr<UObject> Data = nullptr;
};

struct FUIExtensionPoint : TSharedFromThis<FUIExtensionPoint>
{
	FGameplayTag ExtensionPointTag;
	TWeakObjectPtr<UObject> ContextObject;
	EUIExtensionPointMatch ExtensionPointTagMatchType{EUIExtensionPointMatch::ExactMatch};
	TArray<TObjectPtr<UClass>> AllowedDataClasses;
	FExtendExtensionPointDelegate Callback;

	bool DoesExtensionPassContract(const FUIExtension* Extension) const;
};

USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionPointHandle
{
	GENERATED_BODY()

	FUIExtensionPointHandle()
	{
	}

	void Unregister();

	bool IsValid() const { return DataPtr.IsValid(); }

	bool operator==(const FUIExtensionPointHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FUIExtensionPointHandle& Other) const { return !operator==(Other); }

	friend uint32 GetTypeHash(const FUIExtensionPointHandle& Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}

private:
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;

	TSharedPtr<FUIExtensionPoint> DataPtr;

	friend UUIExtensionSubsystem;

	FUIExtensionPointHandle(UUIExtensionSubsystem* InExtensionSource,
	                        const TSharedPtr<FUIExtensionPoint>& InDataPtr) : ExtensionSource(InExtensionSource),
	                                                                          DataPtr(InDataPtr)
	{
	}
};

template <>
struct TStructOpsTypeTraits<FUIExtensionPointHandle> : public TStructOpsTypeTraitsBase2<FUIExtensionPointHandle>
{
	enum
	{
		WithCopy = true,
		WithIdenticalViaEquality = true,
	};
};

USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionHandle
{
	GENERATED_BODY()

	FUIExtensionHandle()
	{
	}

	void Unregister();

	bool IsValid() const { return DataPtr.IsValid(); }

	bool operator==(const FUIExtensionHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FUIExtensionHandle& Other) const { return !operator==(Other); }

	friend FORCEINLINE uint32 GetTypeHash(FUIExtensionHandle Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}

private:
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;

	TSharedPtr<FUIExtension> DataPtr;

	friend UUIExtensionSubsystem;

	FUIExtensionHandle(UUIExtensionSubsystem* InExtensionSource,
	                   const TSharedPtr<FUIExtension>& InDataPtr) : ExtensionSource(InExtensionSource),
	                                                                DataPtr(InDataPtr)
	{
	}
};

template <>
struct TStructOpsTypeTraits<FUIExtensionHandle> : public TStructOpsTypeTraitsBase2<FUIExtensionHandle>
{
	enum
	{
		WithCopy = true,
		WithIdenticalViaEquality = true,
	};
};

USTRUCT(BlueprintType)
struct FUIExtensionRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUIExtensionHandle ExtensionHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ExtensionPointTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> Data{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> ContextObject{nullptr};
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FExtendExtensionPointDynamicDelegate, EUIExtensionAction, Action,
                                   const FUIExtensionRequest&, ExtensionRequest);

UCLASS()
class UIEXTENSION_API UUIExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:	
	FUIExtensionPointHandle RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag,
	                                               EUIExtensionPointMatch ExtensionPointTagMatchType,
	                                               const TArray<UClass*>& AllowedDataClasses,
	                                               FExtendExtensionPointDelegate ExtensionCallback);
	FUIExtensionPointHandle RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag,
	                                                         UObject* ContextObject,
	                                                         EUIExtensionPointMatch ExtensionPointTagMatchType,
	                                                         const TArray<UClass*>& AllowedDataClasses,
	                                                         FExtendExtensionPointDelegate ExtensionCallback);

	FUIExtensionHandle RegisterExtensionAsWidget(const FGameplayTag& ExtensionPointTag,
	                                             TSubclassOf<UUserWidget> WidgetClass, int32 Priority);
	FUIExtensionHandle RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag,
	                                                       UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass,
	                                                       int32 Priority);
	FUIExtensionHandle RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject,
	                                           UObject* Data, int32 Priority);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	void UnregisterExtension(const FUIExtensionHandle& ExtensionHandle);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	void UnregisterExtensionPoint(const FUIExtensionPointHandle& ExtensionPointHandle);

	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

protected:
	void NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& ExtensionPoint);
	void NotifyExtensionPointsOfExtension(EUIExtensionAction Action, TSharedPtr<FUIExtension>& Extension);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="UI Extension",
		meta = (DisplayName = "Register Extension Point"))
	FUIExtensionPointHandle K2_RegisterExtensionPoint(FGameplayTag ExtensionPointTag,
	                                                  EUIExtensionPointMatch ExtensionPointTagMatchType,
	                                                  const TArray<UClass*>& AllowedDataClasses,
	                                                  FExtendExtensionPointDynamicDelegate ExtensionCallback);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension",
		meta = (DisplayName = "Register Extension (Widget)"))
	FUIExtensionHandle K2_RegisterExtensionAsWidget(FGameplayTag ExtensionPointTag,
	                                                TSubclassOf<UUserWidget> WidgetClass, int32 Priority = -1);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension",
		meta = (DisplayName = "Register Extension (Widget For Context)"))
	FUIExtensionHandle K2_RegisterExtensionAsWidgetForContext(FGameplayTag ExtensionPointTag,
	                                                          TSubclassOf<UUserWidget> WidgetClass,
	                                                          UObject* ContextObject, int32 Priority = -1);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="UI Extension",
		meta = (DisplayName = "Register Extension (Data)"))
	FUIExtensionHandle K2_RegisterExtensionAsData(FGameplayTag ExtensionPointTag, UObject* Data, int32 Priority = -1);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="UI Extension",
		meta = (DisplayName = "Register Extension (Data For Context)"))
	FUIExtensionHandle K2_RegisterExtensionAsDataForContext(FGameplayTag ExtensionPointTag, UObject* ContextObject,
	                                                        UObject* Data, int32 Priority = -1);

	FUIExtensionRequest CreateExtensionRequest(const TSharedPtr<FUIExtension>& Extension);

	typedef TArray<TSharedPtr<FUIExtensionPoint>> FExtensionPointList;
	TMap<FGameplayTag, FExtensionPointList> ExtensionPointMap;

	typedef TArray<TSharedPtr<FUIExtension>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;
};

UCLASS()
class UIEXTENSION_API UUIExtensionHandleFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UUIExtensionHandleFunctions()
	{
	}

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	static void Unregister(UPARAM(ref) FUIExtensionHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	static bool IsValid(UPARAM(ref) FUIExtensionHandle& Handle);
};


UCLASS()
class UIEXTENSION_API UUIExtensionPointHandleFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UUIExtensionPointHandleFunctions()
	{
	}

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	static void Unregister(UPARAM(ref) FUIExtensionPointHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	static bool IsValid(UPARAM(ref) FUIExtensionPointHandle& Handle);
};
