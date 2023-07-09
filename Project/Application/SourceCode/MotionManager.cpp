#include "MotionManager.h"

std::unordered_map<std::string, std::unique_ptr<Motion>> MotionManager::sMotionMap;

Motion* MotionManager::Load(const std::string filename, const std::string tag)
{
	// ファイルストリーム
	std::ifstream file;

	std::string directoryPath = "Application/Data/MotionData/";
	std::string format = ".json";
	std::string fullPath = directoryPath + filename + format;

	// ファイルを開く
	file.open(fullPath);
	// ファイルオープン失敗をチェックs
	if (file.fail())
	{
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルエディターデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルエディターデータファイルかチェック
	assert(name.compare("scene") == 0);

	std::unique_ptr<Motion> motion = std::make_unique<Motion>();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["object"])
	{
		motion->data.emplace_back();
		ParseRecursive(&motion->data.back(), object);
	}

	// mapに格納
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

	// トランスフォームのパラメータ読み込み
	nlohmann::json transform = jsonObj["transform"];

	// 回転角
	data->endRots.emplace_back();
	data->endRots.back().x = Radian((float)transform["rotation"][0]);
	data->endRots.back().y = Radian((float)transform["rotation"][1]);
	data->endRots.back().z = Radian((float)transform["rotation"][2]);

	if (jsonObj.contains("parameter"))
	{
		// エージング
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

	// 子があれば
	if (jsonObj.contains("children"))
	{
		nlohmann::json children = jsonObj["children"];
		for (const auto& child : children)
		{
			ParseRecursive(data, child);
		}
	}
}