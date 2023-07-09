#include "StageLoader.h"

std::unordered_map<std::string, std::unique_ptr<StageData>> StageLoader::sMotionMap;
BlockType StageLoader::sCurrentBlockType;
StageType StageLoader::sCurrentStageType;

StageData* StageLoader::LoadStage(const std::string filename, const std::string tag)
{
	// ファイルストリーム
	std::ifstream file;

	std::string directoryPath = "Application/Data/";
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

	std::unique_ptr<StageData> stageData = std::make_unique<StageData>();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["object"])
	{
		ParseRecursive(stageData.get(), object);
	}

	// mapに格納
	sMotionMap.insert(std::make_pair(tag, std::move(stageData)));
	return sMotionMap[tag].get();
}

void StageLoader::ParseRecursive(StageData* data, nlohmann::json jsonObj)
{
	std::string name = jsonObj["name"].get<std::string>();
	data->name_ = name;

	if (jsonObj.contains("parent_name"))
	{

		std::string praentName = jsonObj["parent_name"].get<std::string>();
		praentName = ExtractName(praentName);

		if (praentName != "not_find")
		{
			BlockType type = GetBlockType(praentName);

			if (type != BlockType::None)
			{
				data->blocks_.emplace_back();
				data->blocks_.back() = std::move(GetBlockInstance(type));

				// トランスフォームのパラメータ読み込み
				nlohmann::json transform = jsonObj["transform"];

				auto& current = data->blocks_.back();
				current->block_->pos.x = transform["translation"][0];
				current->block_->pos.y = transform["translation"][1];
				current->block_->pos.z = transform["translation"][2];

				current->block_->scale.x = Radian((float)transform["scaling"][0]);
				current->block_->scale.y = Radian((float)transform["scaling"][1]);
				current->block_->scale.z = Radian((float)transform["scaling"][2]);
			}

		}
	}


	sCurrentBlockType = GetBlockType(name);
	if (sCurrentBlockType != BlockType::None)
	{


		//// 子があれば
		//if (jsonObj.contains("children"))
		//{
		//	nlohmann::json children = jsonObj["children"];
		//	for (const auto& child : children)
		//	{
		//		ParseRecursive(data, child);
		//	}
		//}
	}
	// エリアのクラスを作成する（後で）
	//else
	//{
	//	sCurrentStageType = GetStageType(name);
	//}

	//// ブロックの種類
	//if (jsonObj.contains("block_type"))
	//{
	//	// トランスフォームのパラメータ読み込み
	//	nlohmann::json transform = jsonObj["transform"];

	//	BlockType blockType = (BlockType)jsonObj["block_type"];
	//	data->blocks_.emplace_back();
	//	data->blocks_.back() = std::move(GetBlockInstance(name));
	//}

	//if (jsonObj.contains("block_type"))
	//{
	//	// 回転角
	//	data->endRots.emplace_back();
	//	data->endRots.back().x = Radian((float)transform["rotation"][0]);
	//	data->endRots.back().y = Radian((float)transform["rotation"][1]);
	//	data->endRots.back().z = Radian((float)transform["rotation"][2]);

	//}

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

std::string StageLoader::ExtractName(const std::string input)
{
	std::string result = input;
	std::vector<std::string> list =
	{
		"Block",// "ShaveBlock", "BigBlock"
	};

	std::size_t found;
	for (const auto cur : list)
	{
		found = input.find(cur);
		if (found != std::string::npos)
		{
			result.erase(found + cur.size());
			return result;
		}
	}

	return "not_find";
}

BlockType StageLoader::GetBlockType(const std::string blockType)
{

	if (blockType == "Block")
	{
		return BlockType::Block;
	}
	else
	{
		return BlockType::None;
	}

	return BlockType::None;
}

StageType StageLoader::GetStageType(const std::string stageType)
{
	if (stageType == "Flower1")
	{
		return StageType::Flower1;
	}
	else
	{
		return StageType::None;
	}

	return StageType::None;
}

std::unique_ptr<IBlock> StageLoader::GetBlockInstance(const BlockType blockType)
{
	switch (blockType)
	{
	case BlockType::Block:
		return std::make_unique<Block>();

	default:
		return nullptr;

	}

	return nullptr;
}
