#include "MotionManager.h"

std::unordered_map<std::string, std::unique_ptr<Motion>> MotionManager::sMotionMap;

Motion* MotionManager::Load(const std::string filename, const std::string tag)
{
	// 繝輔ぃ繧､繝ｫ繧ｹ繝医Μ繝ｼ繝
	std::ifstream file;

	std::string directoryPath = "Application/Data/MotionData/";
	std::string format = ".json";
	std::string fullPath = directoryPath + filename + format;

	// 繝輔ぃ繧､繝ｫ繧帝幕縺・
	file.open(fullPath);
	// 繝輔ぃ繧､繝ｫ繧ｪ繝ｼ繝励Φ螟ｱ謨励ｒ繝√ぉ繝・けs
	if (file.fail())
	{
		assert(0);
	}

	// JSON譁・ｭ怜・縺九ｉ隗｣蜃阪＠縺溘ョ繝ｼ繧ｿ
	nlohmann::json deserialized;

	// 隗｣蜃・
	file >> deserialized;

	// 豁｣縺励＞繝ｬ繝吶Ν繧ｨ繝・ぅ繧ｿ繝ｼ繝・・繧ｿ繝輔ぃ繧､繝ｫ縺九メ繧ｧ繝・け
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"繧呈枚蟄怜・縺ｨ縺励※蜿門ｾ・
	std::string name = deserialized["name"].get<std::string>();
	// 豁｣縺励＞繝ｬ繝吶Ν繧ｨ繝・ぅ繧ｿ繝ｼ繝・・繧ｿ繝輔ぃ繧､繝ｫ縺九メ繧ｧ繝・け
	assert(name.compare("scene") == 0);

	std::unique_ptr<Motion> motion = std::make_unique<Motion>();

	// "objects"縺ｮ蜈ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ襍ｰ譟ｻ
	for (nlohmann::json& object : deserialized["object"])
	{
		motion->data.emplace_back();
		ParseRecursive(&motion->data.back(), object);
	}

	// map縺ｫ譬ｼ邏・
	sMotionMap.insert(std::make_pair(tag, std::move(motion)));
	return sMotionMap[tag].get();
}

Motion* MotionManager::GetMotion(const std::string tag)
{
	return sMotionMap[tag].get();
}

void MotionManager::ParseRecursive(MotionData* data, nlohmann::json jsonObj)
{
	std::string name = jsonObj["name"].get<std::string>();
	data->name.emplace_back(name);

	// 繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝縺ｮ繝代Λ繝｡繝ｼ繧ｿ隱ｭ縺ｿ霎ｼ縺ｿ
	nlohmann::json transform = jsonObj["transform"];
	if (jsonObj.contains("is_weapon"))
	{
		data->endWeaponPoses.emplace_back();
		data->endWeaponPoses.back().x = (float)transform["translation"][0];
		data->endWeaponPoses.back().y = (float)transform["translation"][1];
		data->endWeaponPoses.back().z = (float)transform["translation"][2];

		data->endWeaponRots.emplace_back();
		data->endWeaponRots.back().x = Radian((float)transform["rotation"][0]);
		data->endWeaponRots.back().y = Radian((float)transform["rotation"][1]);
		data->endWeaponRots.back().z = Radian((float)transform["rotation"][2]);
	}
	else
	{
		// 蝗櫁ｻ｢隗・
		data->endRots.emplace_back();
		data->endRots.back().x = Radian((float)transform["rotation"][0]);
		data->endRots.back().y = Radian((float)transform["rotation"][1]);
		data->endRots.back().z = Radian((float)transform["rotation"][2]);
	}

	if (jsonObj.contains("parameter"))
	{
		// 繧ｨ繝ｼ繧ｸ繝ｳ繧ｰ
		nlohmann::json parameter = jsonObj["parameter"];
		std::string easeType = parameter["ease_type"].get<std::string>();
		if (easeType == "Lerp")
		{
			data->ease.SetEaseType(EaseType::Lerp);
		}
		else if (easeType == "In")
		{
			data->ease.SetEaseType(EaseType::In);
		}
		else if (easeType == "Out")
		{
			data->ease.SetEaseType(EaseType::Out);
		}
		else if (easeType == "InOut")
		{
			data->ease.SetEaseType(EaseType::InOut);
		}
		data->ease.SetPowNum(parameter["ease_pow"]);
		data->ease.SetEaseTimer(parameter["ease_time"]);
	}

	// 蟄舌′縺ゅｌ縺ｰ
	if (jsonObj.contains("children"))
	{
		nlohmann::json children = jsonObj["children"];
		for (const auto& child : children)
		{
			ParseRecursive(data, child);
		}
	}
}
