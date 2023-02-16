#include "ModelManager.h"
#include "FbxLoader.h"

std::map<std::string, std::unique_ptr<Model>> ModelManager::modelMap;

Model* ModelManager::GetModel(const std::string& modelTag)
{
	return modelMap[modelTag].get();
}

Model* ModelManager::LoadObjModel(const std::string& filePath, const std::string& modelTag, const bool& isSmoothing)
{
	std::unique_ptr<Model> model;
	model.reset(new Model(filePath, isSmoothing));
	modelMap.insert(std::make_pair(modelTag, std::move(model)));

	return modelMap[modelTag].get();
}

Model* ModelManager::LoadFbxModel(const std::string& filePath, const std::string& modelTag)
{
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
	const size_t nodeCount = fbxScene->GetNodeCount();
	model->nodes.reserve(nodeCount);

	// ルートノードから順に解析してモデルに流し込む
	FbxLoader::GetInstance()->ParseNodeRecursive(model.get(), fbxScene->GetRootNode());
	// fbxシーンの解放
	fbxScene->Destroy();

	model->mesh.vertexBuffer.Create(model->mesh.vertices);
	model->mesh.indexBuffer.Create(model->mesh.indices);

	modelMap.insert(std::make_pair(modelTag, std::move(model)));

	return modelMap[modelTag].get();
}