#include "AssimpLoader.h"
#include "TextureManager.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

std::string ExractFileName(const std::string& path)
{
	size_t pos1;
	// 区切り文字 '\\' が出てくる一番最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}

void AssimpLoader::LoadFbxModel(const std::string filePath, FbxModel* model)
{
	// インポーター
	Assimp::Importer importer;

	// フラグ
	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;

	// シーンの読み込み
	const aiScene* scene = importer.ReadFile(filePath, flags);

	if (scene == nullptr)
	{
		assert(0 && "モデルの読み込みが失敗しました");
	}

	// メッシュの解析
	ParseMesh(model, scene);

	// マテリアルの解析
	ParseMaterial(model, scene);
}

void AssimpLoader::ParseMesh(FbxModel* model, const aiScene* scene)
{
	ParseMeshVertices(model, *scene->mMeshes);
	ParseMeshFaces(model, *scene->mMeshes);
}
void AssimpLoader::ParseMeshVertices(FbxModel* model, aiMesh* mesh)
{
	std::vector<VertexPosNormalUvBone>& vertex = model->mesh.vertices;
	vertex.resize(mesh->mNumVertices);

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		// 頂点座標
		vertex[i].pos.x = mesh->mVertices[i].x;
		vertex[i].pos.y = mesh->mVertices[i].y;
		vertex[i].pos.z = mesh->mVertices[i].z;

		// 法線ベクトル
		vertex[i].normal.x = mesh->mNormals[i].x;
		vertex[i].normal.y = mesh->mNormals[i].y;
		vertex[i].normal.z = mesh->mNormals[i].z;

		// uv座標
		vertex[i].uv.x = mesh->mTextureCoords[0][i].x;
		vertex[i].uv.y = mesh->mTextureCoords[0][i].y;
	}
}
void AssimpLoader::ParseMeshFaces(FbxModel* model, aiMesh* mesh)
{
	std::vector<uint16_t>& indices = model->mesh.indices;

	// フェンス
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		if (face.mNumIndices == 3) // 三角形の場合
		{
			unsigned int index1 = face.mIndices[0];
			unsigned int index2 = face.mIndices[1];
			unsigned int index3 = face.mIndices[2];

			if (i % 2 == 0)
			{
				indices.push_back(index1);
				indices.push_back(index2);
				indices.push_back(index3);
			}
			else
			{
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index1);
			}
		}
		else if (face.mNumIndices == 4) // 四角形の場合
		{
			unsigned int index1 = face.mIndices[0];
			unsigned int index2 = face.mIndices[1];
			unsigned int index3 = face.mIndices[2];
			unsigned int index4 = face.mIndices[3];

			indices.push_back(index2);
			indices.push_back(index3);
			indices.push_back(index4);
			indices.push_back(index1);
		}
	}
}
void AssimpLoader::ParseMaterial(FbxModel* model, const aiScene* scene)
{
	for (uint32_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		// マテリアル名
		aiString materialName;
		if (material->Get(AI_MATKEY_NAME, materialName) == AI_SUCCESS)
		{
			model->material.name = materialName.C_Str();
		}

		// アンビエントカラー
		aiColor3D ambientColor;
		if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS)
		{
			model->material.ambient.r = ambientColor.r;
			model->material.ambient.g = ambientColor.g;
			model->material.ambient.b = ambientColor.b;
		}

		// ディフューズカラー
		aiColor3D diffuseColor;
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
		{
			model->material.diffuse.r = diffuseColor.r;
			model->material.diffuse.g = diffuseColor.g;
			model->material.diffuse.b = diffuseColor.b;
		}

		// テクスチャー
		int textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
		for (int i = 0; i < textureCount; i++)
		{
			aiString texturePath;
			//aiTextureMapping textureMapping;
			//unsigned int uvIndex;
			//float blendFactor;
			//aiTextureOp textureOp;
			//aiTextureMapMode mapModeU, mapModeV;

			if (material->GetTexture(aiTextureType_DIFFUSE, i, &texturePath) == AI_SUCCESS)
			{
				const std::string filePath = texturePath.C_Str();

				std::string pathStr(filePath);
				std::string name = ExractFileName(pathStr);
				std::string baseDirectory = "Application/Resources/Model/";
				std::string fullPath = baseDirectory + model->name + "/" + name;

				// テクスチャ読み込み
				model->texture = TextureManager::LoadMaterialTexture(fullPath);

				//std::cout << "Diffuse Texture " << i << ": " << texturePath.C_Str() << std::endl;
				//std::cout << "  Mapping: " << textureMapping << std::endl;
				//std::cout << "  UV Index: " << uvIndex << std::endl;
				//std::cout << "  Blend Factor: " << blendFactor << std::endl;
				//std::cout << "  Texture Operation: " << textureOp << std::endl;
				//std::cout << "  Map Mode U: " << mapModeU << std::endl;
				//std::cout << "  Map Mode V: " << mapModeV << std::endl;
			}
		}
	}
}
