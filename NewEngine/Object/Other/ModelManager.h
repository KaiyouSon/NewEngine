#pragma once
#include "Model.h"
#include "ObjModel.h"
#include "FbxModel.h"
#include <unordered_map>

class ModelManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<Model>> sModelMap_;	// モデルのマップ
	static std::mutex sMtx_;	// 排他制御
	static std::string sDirectoryPath_;

public:	// モデル関連
	// モデルの取得
	static Model* GetModel(const std::string modelTag);

	// objファイルからモデルをロードしマップの格納する
	static Model* LoadObjModel(const std::string fileName, const std::string modelTag, const bool isSmoothing = false);

	// fbxファイルからモデルをロードしマップの格納する
	static Model* LoadFbxModel(const std::string fileName, const std::string modelTag);

	static void LoadMaterialColor(std::string filePath, Model* model);
};