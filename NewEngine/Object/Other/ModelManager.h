#pragma once
#include "Model.h"
#include "FbxModel.h"
#include <unordered_map>

class ModelManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<Model>> sModelMap_;	// モデルのマップ
	static std::mutex sMtx_;	// 排他制御

public:	// モデル関連
	// モデルの取得
	static Model* GetModel(std::string modelTag);

	// objファイルからモデルをロードしマップの格納する
	static Model* LoadObjModel(std::string filePath, std::string modelTag, bool isSmoothing = false);

	// fbxファイルからモデルをロードしマップの格納する
	static Model* LoadFbxModel(std::string filePath, std::string modelTag);

public:	// その他の処理
	// 破棄
	static void Destroy();
};