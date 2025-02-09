#pragma once

#include "CommonListView.h"
#include "DodListView.generated.h"

class UDodWidgetFactory;

UCLASS(meta = (DisableNativeTick))
class DOD_API UDodListView : public UCommonListView
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& InCompileLog) const override;
#endif

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
	                                                   const TSharedRef<STableViewBase>& OwnerTable) override;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category="Entry Creation")
	TArray<TObjectPtr<UDodWidgetFactory>> FactoryRules;
};
