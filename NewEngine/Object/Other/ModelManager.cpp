#include "ModelManager.h"
#include "FbxLoader.h"
#include "AssimpLoader.h"

#pragma region 静的関数

// モデルのマップ
std::unordered_map<std::string, std::unique_ptr<Model>> ModelManager::sModelMap_;
std::unordered_map<std::string, std::unique_ptr<FbxModel>> ModelManager::sFbxModelMap_;
std::mutex ModelManager::sMtx_ = std::mutex{};

#pragma endregion

#pragma region モデル関連

// モデルの取得
Model* ModelManager::GetModel(std::string modelTag)
{
	return sModelMap_[modelTag].get();
}

// objファイルからモデルをロードしマップの格納する
Model* ModelManager::LoadObjModel(std::string filePath, std::string modelTag, bool isSmoothing)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(sMtx_);

	std::unique_ptr<Model> model;
	model = std::make_unique<Model>(filePath, isSmoothing);
	sModelMap_.insert(std::make_pair(modelTag, std::move(model)));

	return sModelMap_[modelTag].get();
}

Model* ModelManager::LoadFbxModel2(std::string filePath, std::string modelTag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(sMtx_);

	// モデル生成
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();

	// モデルと同じ名前のフォルダーから読み込む
	std::string path = "Application/Resources/Model/" + filePath + "/";
	std::string fbxfile = filePath + ".fbx";
	std::string fullPath = path + fbxfile;

	//if (!FbxLoader::GetInstance()->GetFbxImporter()->Initialize(
	//	fullPath.c_str(), -1, FbxLoader::GetInstance()->GetFbxManager()->GetIOSettings()))
	//{
	//	assert(0 && "モデルの読み込みが失敗しました");
	//}

	AssimpLoader::GetInstance()->LoadFbxModel(model.get());

	//// シーン生成
	//FbxScene* fbxScene = FbxScene::Create(FbxLoader::GetInstance()->GetFbxManager(), "fbxScene");

	//// ファイルからロードしたFBXの情報をシーンインポート
	//FbxLoader::GetInstance()->GetFbxImporter()->Import(fbxScene);

	//model->name = filePath;

	//// fbxノード数を取得して必要サイズ分メモリ確保する
	//size_t nodeCount = fbxScene->GetNodeCount();
	//model->nodes.reserve(nodeCount);

	//// ルートノードから順に解析してモデルに流し込む
	//FbxLoader::GetInstance()->ParseNodeRecursive(model.get(), fbxScene->GetRootNode());
	//// fbxシーンの解放
	////fbxScene->Destroy();
	//model->fbxScene = fbxScene;

	model->mesh.vertexBuffer.Create(model->mesh.vertices);
	model->mesh.indexBuffer.Create(model->mesh.indices);

	sModelMap_.insert(std::make_pair(modelTag, std::move(model)));

	return sModelMap_[modelTag].get();
}

Model* ModelManager::LoadFbxModel(std::string filePath, std::string modelTag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(sMtx_);

	// モデル生成
	std::unique_ptr<FbxModel1> model = std::make_unique<FbxModel1>();

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

	sModelMap_.insert(std::make_pair(modelTag, std::move(model)));

	return sModelMap_[modelTag].get();
}

#pragma endregion

#pragma region その他の処理

void ModelManager::Destroy()
{
	for (auto& model : sModelMap_)
	{
		if (model.second->modelType == "FBX")
		{
			static_cast<FbxModel1*>(model.second.get())->fbxScene->Destroy();
		}
	}
}

#pragma endregion