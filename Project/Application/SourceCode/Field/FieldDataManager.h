#pragma once
#include "NewEngine.h"
#include "FieldData.h"
#include "json.hpp"

// フィールドデータを管理するクラス
class FieldDataManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<FieldData>> sFieldDataMap;

private:
	static void LoadCoffinData(FieldData* data, nlohmann::json jsonObj);
	static void LoadSkyIslandData(FieldData* data, nlohmann::json jsonObj);
	static void LoadTreeData(FieldData* data, nlohmann::json jsonObj);
	static void LoadRespawnPointData(FieldData* data, nlohmann::json jsonObj);
	static void LoadWeedData(FieldData* data, nlohmann::json jsonObj);
	static void LoadWallData(FieldData* data, nlohmann::json jsonObj);
	static void LoadGateData(FieldData* data, nlohmann::json jsonObj);
	static void LoadVolumetricFogData(FieldData* data, nlohmann::json jsonObj);
	static void LoadSunData(FieldData* data, nlohmann::json jsonObj);
	static void LoadAirColliderData(FieldData* data, nlohmann::json jsonObj);
	static void LoadTowerData(FieldData* data, nlohmann::json jsonObj);

public:
	static FieldData* Load(const std::string filename, const std::string tag);
	static FieldData* GetFieldData(const std::string tag);
};

