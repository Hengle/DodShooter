#include "UIExtensionSubsystem.h"

#include "Blueprint/UserWidget.h"

bool FUIExtensionPoint::DoesExtensionPassContract(const FUIExtension* Extension) const
{
	if (UObject* DataPtr = Extension->Data)
	{
		const bool bMatchesContext =
			(ContextObject.IsExplicitlyNull() && Extension->ContextObject.IsExplicitlyNull()) ||
			ContextObject == Extension->ContextObject;

		if (bMatchesContext)
		{
			const UClass* DataClass = DataPtr->IsA(UClass::StaticClass()) ? Cast<UClass>(DataPtr) : DataPtr->GetClass();
			for (const UClass* AllowedDataClass : AllowedDataClasses)
			{
				if (DataClass->IsChildOf(AllowedDataClass) || DataClass->ImplementsInterface(AllowedDataClass))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void FUIExtensionPointHandle::Unregister()
{
	if (UUIExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtensionPoint(*this);
	}
}

void FUIExtensionHandle::Unregister()
{
	if (UUIExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtension(*this);
	}
}

FUIExtensionPointHandle UUIExtensionSubsystem::RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag,
                                                                      EUIExtensionPointMatch ExtensionPointTagMatchType,
                                                                      const TArray<UClass*>& AllowedDataClasses,
                                                                      FExtendExtensionPointDelegate ExtensionCallback)
{
	return RegisterExtensionPointForContext(ExtensionPointTag, nullptr, ExtensionPointTagMatchType, AllowedDataClasses,
	                                        ExtensionCallback);
}

FUIExtensionPointHandle UUIExtensionSubsystem::RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag,
	UObject* ContextObject, EUIExtensionPointMatch ExtensionPointTagMatchType,
	const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback)
{
	if (!ExtensionPointTag.IsValid())
	{
		return FUIExtensionPointHandle();
	}

	if (!ExtensionCallback.IsBound())
	{
		return FUIExtensionPointHandle();
	}

	if (AllowedDataClasses.Num() == 0)
	{
		return FUIExtensionPointHandle();
	}

	FExtensionPointList& List = ExtensionPointMap.FindOrAdd(ExtensionPointTag);

	TSharedPtr<FUIExtensionPoint>& Entry = List.Add_GetRef(MakeShared<FUIExtensionPoint>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->ExtensionPointTagMatchType = ExtensionPointTagMatchType;
	Entry->AllowedDataClasses = AllowedDataClasses;
	Entry->Callback = MoveTemp(ExtensionCallback);

	NotifyExtensionPointOfExtensions(Entry);

	return FUIExtensionPointHandle(this, Entry);
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsWidget(const FGameplayTag& ExtensionPointTag,
                                                                    TSubclassOf<UUserWidget> WidgetClass,
                                                                    int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, nullptr, WidgetClass, Priority);
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag,
                                                                              UObject* ContextObject,
                                                                              TSubclassOf<UUserWidget> WidgetClass,
                                                                              int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, ContextObject, WidgetClass, Priority);
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag,
                                                                  UObject* ContextObject, UObject* Data, int32 Priority)
{
	if (!ExtensionPointTag.IsValid())
	{
		return FUIExtensionHandle();
	}

	if (!Data)
	{
		return FUIExtensionHandle();
	}

	FExtensionList& List = ExtensionMap.FindOrAdd(ExtensionPointTag);

	TSharedPtr<FUIExtension>& Entry = List.Add_GetRef(MakeShared<FUIExtension>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->Data = Data;
	Entry->Priority = Priority;

	NotifyExtensionPointsOfExtension(EUIExtensionAction::Added, Entry);

	return FUIExtensionHandle(this, Entry);
}

void UUIExtensionSubsystem::UnregisterExtension(const FUIExtensionHandle& ExtensionHandle)
{
	if (ExtensionHandle.IsValid())
	{
		checkf(ExtensionHandle.ExtensionSource == this,
		       TEXT("Trying to unregister an extension that's not from this extension subsystem."));

		TSharedPtr<FUIExtension> Extension = ExtensionHandle.DataPtr;
		if (FExtensionList* ListPtr = ExtensionMap.Find(Extension->ExtensionPointTag))
		{
			NotifyExtensionPointsOfExtension(EUIExtensionAction::Removed, Extension);

			ListPtr->RemoveSwap(Extension);

			if (ListPtr->Num() == 0)
			{
				ExtensionMap.Remove(Extension->ExtensionPointTag);
			}
		}
	}
}

void UUIExtensionSubsystem::UnregisterExtensionPoint(const FUIExtensionPointHandle& ExtensionPointHandle)
{
	if (ExtensionPointHandle.IsValid())
	{
		check(ExtensionPointHandle.ExtensionSource == this);

		const TSharedPtr<FUIExtensionPoint> ExtensionPoint = ExtensionPointHandle.DataPtr;
		if (FExtensionPointList* ListPtr = ExtensionPointMap.Find(ExtensionPoint->ExtensionPointTag))
		{
			ListPtr->RemoveSwap(ExtensionPoint);
			if (ListPtr->Num() == 0)
			{
				ExtensionPointMap.Remove(ExtensionPoint->ExtensionPointTag);
			}
		}
	}
}

void UUIExtensionSubsystem::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
	Super::AddReferencedObjects(InThis, Collector);
	
	if (UUIExtensionSubsystem* ExtensionSubsystem = Cast<UUIExtensionSubsystem>(InThis))
	{
		for (auto MapIt = ExtensionSubsystem->ExtensionPointMap.CreateIterator(); MapIt; ++MapIt)
		{
			for (const TSharedPtr<FUIExtensionPoint>& ValueElement : MapIt.Value())
			{
				Collector.AddReferencedObjects(ValueElement->AllowedDataClasses);
			}
		}

		for (auto MapIt = ExtensionSubsystem->ExtensionMap.CreateIterator(); MapIt; ++MapIt)
		{
			for (const TSharedPtr<FUIExtension>& ValueElement : MapIt.Value())
			{
				Collector.AddReferencedObject(ValueElement->Data);
			}
		}
	}
}

void UUIExtensionSubsystem::NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& ExtensionPoint)
{
	for (FGameplayTag Tag = ExtensionPoint->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FExtensionList* ListPtr = ExtensionMap.Find(Tag))
		{
			// Copy in case there are removals while handling callbacks
			FExtensionList ExtensionArray(*ListPtr);

			for (const TSharedPtr<FUIExtension>& Extension : ExtensionArray)
			{
				if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
				{
					FUIExtensionRequest Request = CreateExtensionRequest(Extension);
					ExtensionPoint->Callback.ExecuteIfBound(EUIExtensionAction::Added, Request);
				}
			}
		}

		if (ExtensionPoint->ExtensionPointTagMatchType == EUIExtensionPointMatch::ExactMatch)
		{
			break;
		}
	}
}

void UUIExtensionSubsystem::NotifyExtensionPointsOfExtension(EUIExtensionAction Action,
                                                             TSharedPtr<FUIExtension>& Extension)
{
	bool bOnInitialTag = true;
	for (FGameplayTag Tag = Extension->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FExtensionPointList* ListPtr = ExtensionPointMap.Find(Tag))
		{
			// Copy in case there are removals while handling callbacks
			FExtensionPointList ExtensionPointArray(*ListPtr);

			for (const TSharedPtr<FUIExtensionPoint>& ExtensionPoint : ExtensionPointArray)
			{
				if (bOnInitialTag || (ExtensionPoint->ExtensionPointTagMatchType ==
					EUIExtensionPointMatch::PartialMatch))
				{
					if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
					{
						FUIExtensionRequest Request = CreateExtensionRequest(Extension);
						ExtensionPoint->Callback.ExecuteIfBound(Action, Request);
					}
				}
			}
		}

		bOnInitialTag = false;
	}
}

FUIExtensionPointHandle UUIExtensionSubsystem::K2_RegisterExtensionPoint(FGameplayTag ExtensionPointTag,
                                                                         EUIExtensionPointMatch
                                                                         ExtensionPointTagMatchType,
                                                                         const TArray<UClass*>& AllowedDataClasses,
                                                                         FExtendExtensionPointDynamicDelegate
                                                                         ExtensionCallback)
{
	return RegisterExtensionPoint(ExtensionPointTag, ExtensionPointTagMatchType, AllowedDataClasses,
	                              FExtendExtensionPointDelegate::CreateWeakLambda(
		                              ExtensionCallback.GetUObject(),
		                              [this, ExtensionCallback](EUIExtensionAction Action,
		                                                        const FUIExtensionRequest& Request)
		                              {
			                              ExtensionCallback.ExecuteIfBound(Action, Request);
		                              }));
}

FUIExtensionHandle UUIExtensionSubsystem::K2_RegisterExtensionAsWidget(FGameplayTag ExtensionPointTag,
                                                                       TSubclassOf<UUserWidget> WidgetClass,
                                                                       int32 Priority)
{
	return RegisterExtensionAsWidget(ExtensionPointTag, WidgetClass, Priority);
}

FUIExtensionHandle UUIExtensionSubsystem::K2_RegisterExtensionAsWidgetForContext(FGameplayTag ExtensionPointTag,
	TSubclassOf<UUserWidget> WidgetClass, UObject* ContextObject, int32 Priority)
{
	if (ContextObject)
	{
		return RegisterExtensionAsWidgetForContext(ExtensionPointTag, ContextObject, WidgetClass, Priority);
	}
	FFrame::KismetExecutionMessage(
		TEXT("A null ContextObject was passed to Register Extension (Widget For Context)"), ELogVerbosity::Error);
	return FUIExtensionHandle();
}

FUIExtensionHandle UUIExtensionSubsystem::K2_RegisterExtensionAsData(FGameplayTag ExtensionPointTag, UObject* Data,
                                                                     int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, nullptr, Data, Priority);
}

FUIExtensionHandle UUIExtensionSubsystem::K2_RegisterExtensionAsDataForContext(FGameplayTag ExtensionPointTag,
                                                                               UObject* ContextObject, UObject* Data,
                                                                               int32 Priority)
{
	if (ContextObject)
	{
		return RegisterExtensionAsData(ExtensionPointTag, ContextObject, Data, Priority);
	}
	FFrame::KismetExecutionMessage(
		TEXT("A null ContextObject was passed to Register Extension (Data For Context)"), ELogVerbosity::Error);
	return FUIExtensionHandle();
}

FUIExtensionRequest UUIExtensionSubsystem::CreateExtensionRequest(const TSharedPtr<FUIExtension>& Extension)
{
	FUIExtensionRequest Request;
	Request.ExtensionHandle = FUIExtensionHandle(this, Extension);
	Request.ExtensionPointTag = Extension->ExtensionPointTag;
	Request.Priority = Extension->Priority;
	Request.Data = Extension->Data;
	Request.ContextObject = Extension->ContextObject.Get();

	return Request;
}

void UUIExtensionHandleFunctions::Unregister(FUIExtensionHandle& Handle)
{
	Handle.Unregister();
}

bool UUIExtensionHandleFunctions::IsValid(FUIExtensionHandle& Handle)
{
	return Handle.IsValid();
}

void UUIExtensionPointHandleFunctions::Unregister(FUIExtensionPointHandle& Handle)
{
	Handle.Unregister();
}

bool UUIExtensionPointHandleFunctions::IsValid(FUIExtensionPointHandle& Handle)
{
	return Handle.IsValid();
}
