#include "ModelManager.h"
#include "TextureManager.h"
#include "AssimpLoader.h"
#include <fstream>
#include <sstream>

using namespace VertexBufferData;

// モデルのマップ
std::unordered_map<std::string, std::unique_ptr<Model>> ModelManager::sModelMap_;
std::mutex ModelManager::sMtx_ = std::mutex{};
std::string ModelManager::sDirectoryPath_ = "Application/Resources/Model/";

// モデルの取得
Model* ModelManager::GetModel(const std::string modelTag)
{
	return sModelMap_[modelTag].get();
}

// objファイルからモデルをロードしマップの格納する
Model* ModelManager::LoadObjModel(const std::string fileName, const std::string modelTag, const bool isSmoothing)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(sMtx_);

	// インスタンス生成
	std::unique_ptr<Model> model = std::make_unique<ObjModel>();
	model->name = fileName;

	std::string objfile = fileName + ".obj";
	size_t pos;
	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos = fileName.rfind('/');
	if (pos < fileName.size())
	{
		objfile = fileName.substr(pos + 1, fileName.size() - pos - 1) + ".obj";
	}

	std::string path = sDirectoryPath_ + fileName + "/";

	// ファイルストリーム
	std::ifstream file;
	// .objファイルを開く
	file.open(path + objfile);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0 && "モデルの読み込みが失敗しました");
	}

	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> texcoords;

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream lineStream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		std::getline(lineStream, key, ' ');

		if (key == "mtllib")
		{
			// マテリアルのファイル名を読み込む
			std::string fileName;
			lineStream >> fileName;

			LoadMaterialColor(path + fileName, model.get());
			continue;
		}

		// 先頭文字列がvなら頂点座標
		if (key == "v")
		{
			// X,Y,Z座標読み込み
			Vec3 pos{};
			lineStream >> pos.x;
			lineStream >> pos.y;
			lineStream >> pos.z;
			pos.z *= -1;
			// 座標データに追加
			positions.emplace_back(pos);
		}

		// 先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			// uv成分読み込み
			Vec2 texcoord{};
			lineStream >> texcoord.x;
			lineStream >> texcoord.y;
			// v方向反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		// 先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			// xyz成分読み込み
			Vec3 normal{};
			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;
			normal.z *= -1;
			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		// 先頭文字列がfならポリゴン(三角形)
		if (key == "f")
		{
			// 半角スペース区切りで行の続きを読み込む
			std::string indexString;
			int count = 0;

			while (getline(lineStream, indexString, ' '))
			{
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream indexStream(indexString);
				unsigned short indexPos, indexNormal, indexTexcoord;

				indexStream >> indexPos;
				indexStream.seekg(1, std::ios_base::cur);	// スラッシュを飛ばす
				indexStream >> indexTexcoord;
				indexStream.seekg(1, std::ios_base::cur);	// スラッシュを飛ばす

				indexStream >> indexNormal;
				// 頂点データの追加
				VFbxModel vertex{};
				vertex.pos = positions[indexPos - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];

				model->mesh.AddVertex(vertex);

				// 頂点インデックスに追加
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
					model->mesh.AddSmoothData(indexPos, (uint16_t)model->mesh.indices.size() - 1);
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

	// mapに格納
	sModelMap_.insert(std::make_pair(modelTag, std::move(model)));

	return sModelMap_[modelTag].get();
}

// fbxファイルからモデルをロードしマップの格納する
Model* ModelManager::LoadFbxModel(const std::string fileName, const std::string modelTag)
{
	// 排他制御
	std::lock_guard<std::mutex> lock(sMtx_);

	// モデル生成
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();
	model->name = fileName;

	// モデルと同じ名前のフォルダーから読み込む
	std::string path = sDirectoryPath_ + fileName + "/";
	std::string fbxfile = fileName + ".fbx";
	std::string fullPath = path + fbxfile;

	// assimpでロードする
	AssimpLoader::GetInstance()->LoadFbxModel(fullPath, model.get());

	// バッファー生成
	model->mesh.vertexBuffer.Create(model->mesh.vertices);
	model->mesh.indexBuffer.Create(model->mesh.indices);

	// mapに格納
	sModelMap_.insert(std::make_pair(modelTag, std::move(model)));

	return sModelMap_[modelTag].get();
}

// .mtlファイルの読み込み
void ModelManager::LoadMaterialColor(std::string filePath, Model* model)
{
	// ファイルストリーム
	std::ifstream file;
	// .mtlファイルを開く
	file.open(filePath);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0 && "マテリアルの読み込みが失敗しました");
	}

	// ディレクトリパス
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
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream lineStream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		std::getline(lineStream, key, ' ');

		// 先頭のタブ文字を無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			// マテリアル名読み込み
			lineStream >> model->material.name;
		}

		// 先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			lineStream >> model->material.ambient.r;
			lineStream >> model->material.ambient.g;
			lineStream >> model->material.ambient.b;
		}

		// 先頭文字列がKaならディフーズ色
		if (key == "Kd")
		{
			lineStream >> model->material.diffuse.r;
			lineStream >> model->material.diffuse.g;
			lineStream >> model->material.diffuse.b;
		}

		// 先頭文字列がKaならスペキュラー色
		if (key == "Ks")
		{
			lineStream >> model->material.specular.r;
			lineStream >> model->material.specular.g;
			lineStream >> model->material.specular.b;
		}

		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			// テクスチャのファイル名読み込み
			std::string textureName;
			lineStream >> textureName;

			std::string textureTag = model->name + "Texture";

			// テクスチャ読み込み
			model->texture = TextureManager::LoadMaterialTexture(directoryPath + textureName, textureTag);
		}
	}

	// ファイルを閉じる
	file.close();
}
