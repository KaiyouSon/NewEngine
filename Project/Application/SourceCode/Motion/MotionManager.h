#pragma once
#include "NewEngine.h"
#include <json.hpp>

// モーションデータ
struct MotionData
{
	uint32_t partsCount;
	uint32_t step;
	Easing ease;
	std::vector<std::string> name;
	std::vector<Vec3> endRots;
	std::vector<Vec3> endWeaponPoses;
	std::vector<Vec3> endWeaponRots;
};

// モーションのデータを持つ構造体
struct Motion
{
	std::vector<MotionData> data;
};

// 全モーションのデータを管理するクラス
class MotionManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<Motion>> sMotionMap;

private:
	static void ParseRecursive(MotionData* data, nlohmann::json jsonObj);

public:
	static Motion* Load(const std::string filename, const std::string tag);
	static Motion* GetMotion(const std::string tag);
};
