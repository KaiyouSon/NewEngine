#include "ObjModel.h"
namespace fs = std::filesystem;

ObjModel::ObjModel()
{
	format = ModelFormat::Obj;
}

ObjModel::ObjModel(const std::string tag, const std::string& path)
{
	format = ModelFormat::Obj;

	this->tag = tag;
	mPath = path;
}

void ObjModel::Load()
{
	fs::path path = mPath;
	std::string objfile = path.filename().string() + ".obj";
	std::string objFilePath = path.string() + "/" + objfile;
	mMtlFilePath = path.string() + "/" + path.filename().string() + ".mtl";

	// AssimpのImporterオブジェクトを作成
	Assimp::Importer importer;

	// 読み込み
	uint32_t flags = aiProcess_Triangulate | aiProcess_FlipUVs;
	const aiScene* scene = importer.ReadFile(objFilePath, flags);

	// 頂点データの取得
	uint32_t count = 0;
	VertexBufferData::VFbxModel triangle[3];
	for (uint32_t i = 0; i < scene->mMeshes[0]->mNumVertices; ++i)
	{
		triangle[count].pos = GetVertexPos(scene, i);
		triangle[count].uv = GetTexcoords(scene, i);
		triangle[count].normal = GetNormals(scene, i);
		count++;

		if (count == 3)
		{
			for (int32_t j = 2; j >= 0; --j)
			{
				mesh.AddVertex(triangle[j]);
			}
			count = 0;
		}
	}

	// ポリゴンデータを取得
	for (uint32_t i = 0; i < scene->mMeshes[0]->mNumFaces; ++i)
	{
		aiFace face = scene->mMeshes[0]->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			uint16_t index = (uint16_t)face.mIndices[j];
			mesh.AddIndex(index);
		}
	}

	// マテリアルを取得
	for (uint32_t i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial* aiMaterial = scene->mMaterials[i];

		// マテリアルのプロパティにアクセス
		aiColor3D color;

		aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		ambient = Color(color.r, color.g, color.b);

		aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		diffuse = Color(color.r, color.g, color.b);

		aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		specular = Color(color.r, color.g, color.b);

		// テクスチャの読み込み
		uint32_t numTextures = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);

		for (uint32_t j = 0; j < numTextures; ++j)
		{
			aiString aiTextureName;
			aiMaterial->GetTexture(aiTextureType_DIFFUSE, j, &aiTextureName);

			std::string texturePath = mPath + "/" + aiTextureName.C_Str();
			texture = gAssetsManager->LoadMaterialTexture(texturePath);
		}
	}

	mesh.CreateBuffer();

	// マテリアルの読み込み
	//LoadMtlFile();
}

void ObjModel::LoadMtlFile()
{
	// ファイルを開いて読み込み
	std::ifstream file;
	// .mtlファイルを開く
	file.open(mMtlFilePath);
	// ファイルが開けなかった場合
	if (file.fail())
	{
		assert(0 && "マテリアル情報の読み込みに失敗しました");
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
			lineStream >> material.name;
		}

		// Kaキーがあればアンビエントカラーを読み込む
		if (key == "Ka")
		{
			lineStream >> ambient.r;
			lineStream >> ambient.g;
			lineStream >> ambient.b;
		}

		// Kdキーがあればディフューズカラーを読み込む
		if (key == "Kd")
		{
			lineStream >> diffuse.r;
			lineStream >> diffuse.g;
			lineStream >> diffuse.b;
		}

		// Ksキーがあればスペキュラカラーを読み込む
		if (key == "Ks")
		{
			lineStream >> specular.r;
			lineStream >> specular.g;
			lineStream >> specular.b;
		}

		// map_Kdキーがあればテクスチャを読み込む
		if (key == "map_Kd")
		{
			// テクスチャ名を読み込む
			std::string textureName;
			lineStream >> textureName;

			// テクスチャをロード
			std::string texFilePath = mPath + "/" + textureName;
			texture = gAssetsManager->LoadMaterialTexture(texFilePath);
		}
	}

	// ファイルを閉じる
	file.close();
}

Vec3 ObjModel::GetVertexPos(const aiScene* scene, const uint32_t index)
{
	// 頂点座標を取得
	aiVector3D aiPos = scene->mMeshes[0]->mVertices[index];
	return Vec3(aiPos.x, aiPos.y, -aiPos.z);
}

Vec2 ObjModel::GetTexcoords(const aiScene* scene, const uint32_t index)
{
	// uv座標がなければ
	if (!scene->mMeshes[0]->HasTextureCoords(0))
	{
		return 0;
	}

	// テクスチャ座標を取得
	aiVector3D aiTexCoord = scene->mMeshes[0]->mTextureCoords[0][index];
	return Vec2(aiTexCoord.x, aiTexCoord.y);
}

Vec3 ObjModel::GetNormals(const aiScene* scene, const uint32_t index)
{
	// 法線がなければ
	if (!scene->mMeshes[0]->HasNormals())
	{
		return 0;
	}

	aiVector3D aiNormal = scene->mMeshes[0]->mNormals[index];
	return Vec3(aiNormal.x, aiNormal.y, -aiNormal.z);
}