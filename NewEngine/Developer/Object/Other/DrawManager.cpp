#include "DrawManager.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <vector>
using namespace std;

unique_ptr<Texture> LoadTexture(const char* filePath)
{
	return move(TextureBuffer::LoadTexture(filePath));
}

unique_ptr<Model> LoadModelData(const char* filePath)
{
	static uint32_t numberIndex = 0;

	// ファイルストリーム
	ifstream file;
	// .objファイルを開く
	file.open(filePath);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		unique_ptr<Model> errorModel = move(make_unique<Model>());;
		return move(errorModel);
	}

	unique_ptr<Model> modelData = move(make_unique<Model>());
	vector<Vec3> positions;
	vector<Vec3> normals;
	vector<Vec2> texcoords;

	string materialFileName = filePath;
	while (true)
	{
		materialFileName.pop_back();
		if (materialFileName.back() == '/')
		{
			break;
		}
	}

	// 1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream lineStream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(lineStream, key, ' ');

		if (key == "mtllib")
		{
			// マテリアルのファイル名を読み込む
			string filepath;
			lineStream >> filepath;

			if (filepath.size() > 0)
			{
				materialList->AddMaterial(LoadMaterial(materialFileName + filepath), numberIndex);
				numberIndex++;
			}
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
			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		// 先頭文字列がfならポリゴン(三角形)
		if (key == "f")
		{
			// 半角スペース区切りで行の続きを読み込む
			string indexString;
			while (getline(lineStream, indexString, ' '))
			{
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				istringstream indexStream(indexString);
				unsigned short indexPos, indexNormal, indexTexcoord;
				indexStream >> indexPos;
				indexStream.seekg(1, ios_base::cur);	// スラッシュを飛ばす
				indexStream >> indexTexcoord;
				indexStream.seekg(1, ios_base::cur);	// スラッシュを飛ばす
				indexStream >> indexNormal;
				// 頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPos - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				modelData->vertices.emplace_back(vertex);

				// 頂点インデックスに追加
				modelData->indices.emplace_back((unsigned short)modelData->indices.size());
			}
		}
	}

	// ファイルを閉じる
	file.close();

	return move(modelData);
}

unique_ptr<Material> LoadMaterial(const std::string& filePath)
{
	static uint32_t numberIndex = 0;
	unique_ptr<Material> material = move(make_unique<Material>());

	// ファイルストリーム
	ifstream file;
	// .objファイルを開く
	file.open(filePath);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		//assert(0);
		unique_ptr<Material> errorMaterial = move(make_unique<Material>());
		errorMaterial->name = "error";
		return move(errorMaterial);
	}

	string directoryPath = filePath;
	while (true)
	{
		directoryPath.pop_back();
		if (directoryPath.back() == '/')
		{
			break;
		}
	}

	// 1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream lineStream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(lineStream, key, ' ');

		// 先頭のタブ文字を無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			// マテリアル名読み込み
			lineStream >> material->name;
		}

		// 先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			lineStream >> material->ambient.x;
			lineStream >> material->ambient.y;
			lineStream >> material->ambient.z;
		}

		// 先頭文字列がKaならディフーズ色
		if (key == "Kd")
		{
			lineStream >> material->diffuse.x;
			lineStream >> material->diffuse.y;
			lineStream >> material->diffuse.z;
		}

		// 先頭文字列がKaならスペキュラー色
		if (key == "Kd")
		{
			lineStream >> material->specular.x;
			lineStream >> material->specular.y;
			lineStream >> material->specular.z;
		}

		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			// テクスチャのファイル名読み込み
			lineStream >> material->textureFilepath;

			// テクスチャ読み込み
			//materialTextureList->Add(LoadTexture((directoryPath + material->textureFilepath).c_str()), numberIndex);
			numberIndex++;
		}
	}

	// ファイルを閉じる
	file.close();

	return move(material);
}

void UnLoadTexture(Texture* texture)
{
	TextureBuffer::UnLoadTexture(texture);
}