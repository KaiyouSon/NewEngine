#pragma once
#include "Model.h"
#include "ObjModel.h"
#include "FbxModel.h"
#include <unordered_map>

class ModelManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<Model>> sModelMap;	// 繝｢繝・Ν縺ｮ繝槭ャ繝・
	static std::mutex sMtx;	// 謗剃ｻ門宛蠕｡
	static std::string sDirectoryPath;

public:	// 繝｢繝・Ν髢｢騾｣
	// 繝｢繝・Ν縺ｮ蜿門ｾ・
	static Model* GetModel(const std::string modelTag);

	// obj繝輔ぃ繧､繝ｫ縺九ｉ繝｢繝・Ν繧偵Ο繝ｼ繝峨＠繝槭ャ繝励・譬ｼ邏阪☆繧・
	static Model* LoadObjModel(const std::string fileName, const std::string modelTag, const bool isSmoothing = false);

	// fbx繝輔ぃ繧､繝ｫ縺九ｉ繝｢繝・Ν繧偵Ο繝ｼ繝峨＠繝槭ャ繝励・譬ｼ邏阪☆繧・
	static Model* LoadFbxModel(const std::string fileName, const std::string modelTag);

	static std::unordered_map<std::string, std::unique_ptr<Model>>* GetModelMap();

	static void LoadMaterialColor(std::string filePath, Model* model);
};
