#include "System/DodGameData.h"

#include "System/DodAssetManager.h"

const UDodGameData& UDodGameData::Get()
{
	return UDodAssetManager::Get().GetGameData();
}
