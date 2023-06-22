#include "AssimpLoader.h"
#include "TextureManager.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

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
	for (uint32_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		ParseMeshVertices(model, mesh);
		ParseMeshFaces(model, mesh);
		ParseSkin(model, mesh);
	}
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
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		if (face.mNumIndices == 3) // 三角形の場合
		{
			uint32_t index1 = face.mIndices[0];
			uint32_t index2 = face.mIndices[1];
			uint32_t index3 = face.mIndices[2];

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
			uint32_t index1 = face.mIndices[0];
			uint32_t index2 = face.mIndices[1];
			uint32_t index3 = face.mIndices[2];
			uint32_t index4 = face.mIndices[3];

			indices.push_back(index2);
			indices.push_back(index3);
			indices.push_back(index4);
			indices.push_back(index1);
		}
	}
}
void AssimpLoader::ParseSkin(FbxModel* model, aiMesh* mesh)
{
	// スキニング情報を持つメッシュかどうかを確認します
	if (mesh->HasBones() == true)
	{
		// ボーン番号とスキンウェイトのペア
		struct WeightSet
		{
			UINT index;
			float weight;
		};

		// 二次元配列（ジャグ配列）
		// list:頂点が影響を受けるボーンの全リスト
		// vector:それを全頂点分
		std::vector<std::list<WeightSet>> weightLists(model->mesh.vertices.size());

		// ボーンの最大数設定
		model->bones.resize(mesh->mNumBones);

		// スキニング情報の処理
		for (uint32_t i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* bone = mesh->mBones[i];

			// ボーンの初期姿勢行列(バインドポーズ行列)
			Mat4 initalMat = ConvertMat4FromAssimpMat(bone->mOffsetMatrix);
			model->bones[i].invInitalPose = initalMat.Inverse();

			// ウェイトの読み取り
			for (uint32_t j = 0; j < bone->mNumWeights; j++)
			{
				// 頂点番号
				int vertexIndex = bone->mWeights[j].mVertexId;
				// スキンウェイト
				float weight = bone->mWeights[j].mWeight;
				// その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
				weightLists[vertexIndex].emplace_back(WeightSet{ (UINT)i,weight });
			}
		}

		// ウェイトの整理
		auto& vertices = model->mesh.vertices;
		// 各頂点について処理
		for (uint32_t i = 0; i < mesh->mNumBones; i++)
		{
			// 頂点のウェイトから最も大きい4つを選択
			auto& weightList = weightLists[i];
			// 大小比較用のラムダ式を指定して降順にソート
			weightList.sort(
				[](auto const& lhs, auto const& rhs)
				{
					return lhs.weight > rhs.weight;
				});

			int weightArrayIndex = 0;
			// 降順ソート済みのウェイトリストから
			for (auto& weightSet : weightList)
			{
				// 頂点データに書き込み
				vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
				vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;
				// 4つに達したら終了
				if (++weightArrayIndex >= maxBoneIndices)
				{
					float weight = 0.f;
					// 2番目以降のウェイトを合計
					for (size_t j = 1; j < maxBoneIndices; j++)
					{
						weight += vertices[i].boneWeight[j];
					}
					// 合計で1,f(100%)になるように調整
					vertices[i].boneWeight[0] = 1.f - weight;
					break;
				}
			}
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

			if (material->GetTexture(aiTextureType_DIFFUSE, i, &texturePath) == AI_SUCCESS)
			{
				const std::string filePath = texturePath.C_Str();

				std::string pathStr(filePath);
				std::string name = ExractFileName(pathStr);
				std::string baseDirectory = "Application/Resources/Model/";
				std::string fullPath = baseDirectory + model->name + "/" + name;

				// テクスチャ読み込み
				model->texture = TextureManager::LoadMaterialTexture(fullPath);
			}
		}
	}
}

Mat4 AssimpLoader::ConvertMat4FromAssimpMat(const aiMatrix4x4& mat)
{
	Mat4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = (float)mat[i][j];
		}
	}

	return result;
}
std::string AssimpLoader::ExractFileName(const std::string& path)
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
