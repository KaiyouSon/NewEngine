#pragma once
#include "NewEngine.h"
#include "FieldData.h"
#include "json.hpp"

class FieldDataManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<FieldData>> sFieldDataMap;

private:
	static void LoadCoffinData(FieldData* data, nlohmann::json jsonObj);
	static void LoadSkyIslandData(FieldData* data, nlohmann::json jsonObj);
	static void LoadTreeData(FieldData* data, nlohmann::json jsonObj);
	static void LoadRespawnPointData(FieldData* data, nlohmann::json jsonObj);

public:
	static FieldData* Load(const std::string filename, const std::string tag);
	static FieldData* GetFieldData(const std::string tag);
};

