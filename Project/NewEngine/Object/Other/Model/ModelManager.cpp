#include "ModelManager.h"
#include "AssimpLoader.h"
#include "TextureManager.h"
#include <fstream>
#include <sstream>

using namespace VertexBufferData;
namespace fs = std::filesystem;

ModelManager::ModelManager() :
	mDirectoryPath("Application/Resources/Model/")
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- ロード関連 ---------------------------------------------------------------------------------------------------- ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ModelManager::LoadModel(const std::string fileName, const bool isSmoothing)
{
	isSmoothing;

	fs::path fspath = fileName;
	std::string tag = fspath.filename().string();

	// マップに格納
	std::unique_ptr<Model> model = std::make_unique<ObjModel>(tag, fileName);
	std::pair pair = std::make_pair(tag, std::move(model));
	mModelMap.insert(std::move(pair));

	// キャストする
	ObjModel* objModel = dynamic_cast<ObjModel*>(mModelMap[tag].get());

	objModel->Load();
	//objModel->LoadMtlFile();
}
void ModelManager::LoadModel(const std::wstring fileName, const bool isSmoothing)
{
	isSmoothing;

	fs::path fspath = fileName;
	std::string tag = fspath.filename().string();

	// マップに格納
	std::unique_ptr<Model> model = std::make_unique<ObjModel>(tag, fspath.string());
	std::pair pair = std::make_pair(tag, std::move(model));
	mModelMap.insert(std::move(pair));

	// キャストする
	ObjModel* objModel = dynamic_cast<ObjModel*>(mModelMap[tag].get());

	objModel->Load();
}

// objモデルのロード
Model* ModelManager::LoadObjModel(const std::string fileName, const std::string tag, const bool isSmoothing)
{
	// ロックを取得
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// モデルデータを生成
	std::unique_ptr<Model> model = std::make_unique<ObjModel>();
	model->tag = tag;
	model->name = fileName;

	std::string objfile = fileName + ".obj";
	uint32_t checkPos;
	// ファイルパスの最後に'/'があればファイル名だけにして".obj"を追加
	checkPos = static_cast<uint32_t>(fileName.rfind('/'));
	if (checkPos < fileName.size())
	{
		objfile = fileName.substr(checkPos + 1, fileName.size() - checkPos - 1) + ".obj";
	}

	std::string path = GetInstance()->mDirectoryPath + fileName + "/";

	// ファイルを開いて読み込み
	std::ifstream file;
	// .objファイルを開く
	file.open(path + objfile);
	// ファイルが開けなかった場合
	if (file.fail())
	{
		std::string log = "[ObjModel Load] FileName : " + fileName + ", Tag : " + tag + ", failed to load";
		OutputDebugLog(log.c_str());

		assert(0 && "モデルデータの読み込みに失敗しました");
	}

	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> texcoords;

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		// 1行をスペースで分割
		std::istringstream lineStream(line);

		// 最初の単語を取得して、キーとする
		std::string key;
		std::getline(lineStream, key, ' ');

		if (key == "mtllib")
		{
			// マテリアル情報のファイル名を取得し、カラー情報を読み込む
			std::string mtlFileName;
			lineStream >> mtlFileName;

			LoadMtlFile(path + mtlFileName, model.get());
			continue;
		}

		// ポジション情報を読み込む
		if (key == "v")
		{
			// X,Y,Z座標を読み込む
			Vec3 pos{};
			lineStream >> pos.x;
			lineStream >> pos.y;
			lineStream >> pos.z;
			pos.z *= -1; // Z軸を反転
			// 座標データを配列に追加
			positions.emplace_back(pos);
		}

		// テクスチャ座標情報を読み込む
		if (key == "vt")
		{
			// UV座標を読み込む
			Vec2 texcoord{};
			lineStream >> texcoord.x;
			lineStream >> texcoord.y;
			// Y座標を反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データを配列に追加
			texcoords.emplace_back(texcoord);
		}

		// 法線情報を読み込む
		if (key == "vn")
		{
			// XYZ座標を読み込む
			Vec3 normal{};
			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;
			normal.z *= -1; // Z軸を反転
			// 法線データを配列に追加
			normals.emplace_back(normal);
		}

		// 面情報を読み込む
		if (key == "f")
		{
			// 面情報をスペースで分割
			std::string indexString;
			int count = 0;

			while (getline(lineStream, indexString, ' '))
			{
				// インデックスを解析
				std::istringstream indexStream(indexString);
				uint32_t indexPos = 0, indexNormal = 0, indexTexcoord = 0;

				indexStream >> indexPos;
				indexStream.seekg(1, std::ios_base::cur); // スラッシュをスキップ
				indexStream >> indexTexcoord;
				indexStream.seekg(1, std::ios_base::cur); // スラッシュをスキップ

				indexStream >> indexNormal;
				// ポジション情報、法線情報、テクスチャ座標情報を使って
				// モデルデータを構築
				VFbxModel vertex{};
				vertex.pos = positions[indexPos - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				model->mesh.AddVertex(vertex);

				// インデックスを追加
				if (count % 3 == 0)
				{
					model->mesh.AddIndex((uint16_t)model->mesh.indices.size());
				}
				if (count % 3 == 1)
				{
					model->mesh.AddIndex((uint16_t)model->mesh.indices.size() + 1);
				}
				if (count % 3 == 2)
				{
					model->mesh.AddIndex((uint16_t)model->mesh.indices.size() - 1);
				}

				if (isSmoothing == true)
				{
					model->mesh.AddSmoothData((uint16_t)indexPos, (uint16_t)model->mesh.indices.size() - 1);
				}

				count++;
			}
		}
	}

	// ファイルを閉じる
	file.close();

	if (isSmoothing == true)
	{
		model->mesh.CalculateSmoothedVertexNormals();
	}

	model->mesh.CreateBuffer();

	std::string log = "[ObjModel Load] FileName : " + fileName + ", Tag : " + tag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	// マップに追加
	GetInstance()->mModelMap.insert(std::make_pair(tag, std::move(model)));

	return GetInstance()->mModelMap[tag].get();
}

// fbxファイルの読み込み
Model* ModelManager::LoadFbxModel(const std::string fileName, const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	// Fbxモデルの作成
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();
	model->tag = tag;
	model->name = fileName;

	// Fbxファイルへのパス
	std::string path = GetInstance()->mDirectoryPath + fileName + "/";
	std::string fbxfile = fileName + ".fbx";
	std::string fullPath = path + fbxfile;

	// フラグの設定
	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;

	// シーンの読み込み
	model->scene = model->importer.ReadFile(fullPath, flags);

	if (model->scene == nullptr)
	{
		std::string log = "[FbxModel Load] FileName : " + fileName + ", Tag : " + tag + " の読み込みに失敗しました";
		OutputDebugLog(log.c_str());

		assert(0 && "Fbxファイルの読み込みに失敗しました");
	}

	// マテリアルの解析
	AssimpLoader::GetInstance()->ParseMaterial(model.get(), model->scene);
	AssimpLoader::GetInstance()->ParseNodeRecursive(model.get(), nullptr, model->scene->mRootNode);

	// メッシュの作成
	model->mesh.vertexBuffer.Create(model->mesh.vertices);
	model->mesh.indexBuffer.Create(model->mesh.indices);

	// モデルをマップに追加
	GetInstance()->mModelMap.insert(std::make_pair(tag, std::move(model)));

	std::string log = "[FbxModel Load] FileName : " + fileName + ", Tag : " + tag + " が正常に読み込まれました";
	OutputDebugLog(log.c_str());

	return GetInstance()->mModelMap[tag].get();
}

// .mtlファイルの読み込み
void ModelManager::LoadMtlFile(std::string filePath, Model* model)
{
	// ファイルを開いて読み込み
	std::ifstream file;
	// .mtlファイルを開く
	file.open(filePath);
	// ファイルが開けなかった場合
	if (file.fail())
	{
		assert(0 && "マテリアル情報の読み込みに失敗しました");
	}

	// ファイルのディレクトリパスを取得
	std::string directoryPath = filePath;
	while (true)
	{
		directoryPath.pop_back();
		if (directoryPath.back() == '/')
		{
			break;
		}
	}

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		// 1行をスペースで分割
		std::istringstream lineStream(line);

		// 最初の単語を取得して、キーとする
		std::string key;
		std::getline(lineStream, key, ' ');

		// インデントを除去
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		// newmtlキーがあれば新しいマテリアル情報の開始
		if (key == "newmtl")
		{
			// マテリアル名を読み込む
			lineStream >> model->material.name;
		}

		// Kaキーがあればアンビエントカラーを読み込む
		if (key == "Ka")
		{
			lineStream >> model->material.ambient.r;
			lineStream >> model->material.ambient.g;
			lineStream >> model->material.ambient.b;
		}

		// Kdキーがあればディフューズカラーを読み込む
		if (key == "Kd")
		{
			lineStream >> model->material.diffuse.r;
			lineStream >> model->material.diffuse.g;
			lineStream >> model->material.diffuse.b;
		}

		// Ksキーがあればスペキュラカラーを読み込む
		if (key == "Ks")
		{
			lineStream >> model->material.specular.r;
			lineStream >> model->material.specular.g;
			lineStream >> model->material.specular.b;
		}

		// map_Kdキーがあればテクスチャを読み込む
		if (key == "map_Kd")
		{
			// テクスチャ名を読み込む
			std::string textureName;
			lineStream >> textureName;

			std::string textureTag = model->name + "Texture";

			// テクスチャをロード
			std::string path = directoryPath + textureName;
			std::wstring wpath = std::wstring(path.begin(), path.end());
			model->texture = gAssetsManager->LoadMaterialTexture(wpath);

			//TextureManager::LoadMaterialTexture(wpath, textureTag);
		}
	}

	// ファイルを閉じる
	file.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- 破棄関連 ------------------------------------------------------------------------------------------------------ ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ModelManager::DestroyModel(const std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mModelMap.find(tag);
	if (it == GetInstance()->mModelMap.end())
	{
		std::string log;
		log = "[ObjModel Error] Tag : " + tag + ", is nullptr";
		return;
	}

	// テクスチャのSRV解放
	std::string textureTag = GetInstance()->mModelMap[tag]->name + "Texture";
	TextureManager::DestroyMaterialTexture(textureTag);

	// Mapから削除
	GetInstance()->mModelMap.erase(tag);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- モデルの取得関連 ---------------------------------------------------------------------------------------------- ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Model* ModelManager::GetModel(std::string tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	auto it = GetInstance()->mModelMap.find(tag);
	if (it == GetInstance()->mModelMap.end())
	{
		std::string log;
		log = "[Model Error] Tag : " + tag + ", is nullptr";
		return GetInstance()->mModelMap["Sphere"].get();
	}
	else
	{
		std::string log;
		log = "[Model Use] Tag : " + tag + ", was used";
		OutputDebugLog(log.c_str());

		return GetInstance()->mModelMap[tag].get();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// --- モデルのマップの取得関連 -------------------------------------------------------------------------------------- ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::unordered_map<std::string, std::unique_ptr<Model>>* ModelManager::GetModelMap2()
{
	return &GetInstance()->mModelMap;
}

Model* ModelManager::GetModel2(const std::string& tag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(GetInstance()->mMutex);

	Model* result = nullptr;

	auto it = mModelMap.find(tag);
	if (it != mModelMap.end())
	{
		return mModelMap[tag].get();
	}
	return result;
}

std::unordered_map<std::string, std::unique_ptr<Model>>* ModelManager::GetModelMap()
{
	return &mModelMap;
}

