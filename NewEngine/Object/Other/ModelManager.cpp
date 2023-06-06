#include "ModelManager.h"
#include "FbxLoader.h"

#pragma region 静的関数

// モデルのマップ
std::map<std::string, std::unique_ptr<Model>> ModelManager::modelMap;
std::mutex ModelManager::mtx = std::mutex{};

#pragma endregion

#pragma region モデル関連

// モデルの取得
Model* ModelManager::GetModel(std::string modelTag)
{
	return modelMap[modelTag].get();
}

// objファイルからモデルをロードしマップの格納する
Model* ModelManager::LoadObjModel(std::string filePath, std::string modelTag, bool isSmoothing)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(mtx);

	std::unique_ptr<Model> model;
	model.reset(new Model(filePath, isSmoothing));
	modelMap.insert(std::make_pair(modelTag, std::move(model)));

	return modelMap[modelTag].get();
}

// fbxファイルからモデルをロードしマップの格納する
Model* ModelManager::LoadFbxModel(std::string filePath, std::string modelTag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(mtx);

	// モデル生成
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();

	// モデルと同じ名前のフォルダーから読み込む
	std::string path = "Application/Resources/Model/" + filePath + "/";
	std::string fbxfile = filePath + ".fbx";
	std::string fullPath = path + fbxfile;

	if (!FbxLoader::GetInstance()->GetFbxImporter()->Initialize(
		fullPath.c_str(), -1, FbxLoader::GetInstance()->GetFbxManager()->GetIOSettings()))
	{
		assert(0 && "モデルの読み込みが失敗しました");
	}

	// シーン生成
	FbxScene* fbxScene = FbxScene::Create(FbxLoader::GetInstance()->GetFbxManager(), "fbxScene");

	// ファイルからロードしたFBXの情報をシーンインポート
	FbxLoader::GetInstance()->GetFbxImporter()->Import(fbxScene);

	model->name = filePath;

	// fbxノード数を取得して必要サイズ分メモリ確保する
	size_t nodeCount = fbxScene->GetNodeCount();
	model->nodes.reserve(nodeCount);

	// ルートノードから順に解析してモデルに流し込む
	FbxLoader::GetInstance()->ParseNodeRecursive(model.get(), fbxScene->GetRootNode());
	// fbxシーンの解放
	//fbxScene->Destroy();
	model->fbxScene = fbxScene;

	model->mesh.vertexBuffer.Create(model->mesh.vertices);
	model->mesh.indexBuffer.Create(model->mesh.indices);

	modelMap.insert(std::make_pair(modelTag, std::move(model)));

	return modelMap[modelTag].get();
}

#pragma endregion

#pragma region その他の処理

void ModelManager::Destroy()
{
	for (auto& model : modelMap)
	{
		if (model.second->modelType == "FBX")
		{
			static_cast<FbxModel*>(model.second.get())->fbxScene->Destroy();
		}
	}
}

#pragma endregion