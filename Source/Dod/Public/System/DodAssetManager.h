#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "System/DodGameData.h"
#include "DodAssetManager.generated.h"

UCLASS(Config = Game)
class DOD_API UDodAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UDodAssetManager& Get();

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	const UDodGameData& GetGameData();

protected:
	template <typename GameDataClass>
	const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath);

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	static bool ShouldLogAssetLoads();

	void AddLoadedAsset(const UObject* Asset);

	UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType);

	UPROPERTY(Config)
	TSoftObjectPtr<UDodGameData> DodGameDataPath;

	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;

	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	FCriticalSection LoadedAssetsCritical;
};

template <typename AssetType>
TSubclassOf<AssetType> UDodAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}

template <typename GameDataClass>
const GameDataClass& UDodAssetManager::GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
{
	if (TObjectPtr<UPrimaryDataAsset> const* pResult = GameDataMap.Find(GameDataClass::StaticClass()))
	{
		return *CastChecked<GameDataClass>(*pResult);
	}

	return *CastChecked<const GameDataClass>(
		LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
}
