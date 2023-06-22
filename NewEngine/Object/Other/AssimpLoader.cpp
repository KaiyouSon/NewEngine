#include "AssimpLoader.h"
#include "TextureManager.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void AssimpLoader::LoadFbxModel(const std::string filePath, FbxModel* model)
{
	// �C���|�[�^�[
	Assimp::Importer importer;

	// �t���O
	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;

	// �V�[���̓ǂݍ���
	const aiScene* scene = importer.ReadFile(filePath, flags);

	if (scene == nullptr)
	{
		assert(0 && "���f���̓ǂݍ��݂����s���܂���");
	}

	// ���b�V���̉��
	ParseMesh(model, scene);

	// �}�e���A���̉��
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
		// ���_���W
		vertex[i].pos.x = mesh->mVertices[i].x;
		vertex[i].pos.y = mesh->mVertices[i].y;
		vertex[i].pos.z = mesh->mVertices[i].z;

		// �@���x�N�g��
		vertex[i].normal.x = mesh->mNormals[i].x;
		vertex[i].normal.y = mesh->mNormals[i].y;
		vertex[i].normal.z = mesh->mNormals[i].z;

		// uv���W
		vertex[i].uv.x = mesh->mTextureCoords[0][i].x;
		vertex[i].uv.y = mesh->mTextureCoords[0][i].y;
	}
}
void AssimpLoader::ParseMeshFaces(FbxModel* model, aiMesh* mesh)
{
	std::vector<uint16_t>& indices = model->mesh.indices;

	// �t�F���X
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		if (face.mNumIndices == 3) // �O�p�`�̏ꍇ
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
		else if (face.mNumIndices == 4) // �l�p�`�̏ꍇ
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
	// �X�L�j���O���������b�V�����ǂ������m�F���܂�
	if (mesh->HasBones() == true)
	{
		// �{�[���ԍ��ƃX�L���E�F�C�g�̃y�A
		struct WeightSet
		{
			UINT index;
			float weight;
		};

		// �񎟌��z��i�W���O�z��j
		// list:���_���e�����󂯂�{�[���̑S���X�g
		// vector:�����S���_��
		std::vector<std::list<WeightSet>> weightLists(model->mesh.vertices.size());

		// �{�[���̍ő吔�ݒ�
		model->bones.resize(mesh->mNumBones);

		// �X�L�j���O���̏���
		for (uint32_t i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* bone = mesh->mBones[i];

			// �{�[���̏����p���s��(�o�C���h�|�[�Y�s��)
			Mat4 initalMat = ConvertMat4FromAssimpMat(bone->mOffsetMatrix);
			model->bones[i].invInitalPose = initalMat.Inverse();

			// �E�F�C�g�̓ǂݎ��
			for (uint32_t j = 0; j < bone->mNumWeights; j++)
			{
				// ���_�ԍ�
				int vertexIndex = bone->mWeights[j].mVertexId;
				// �X�L���E�F�C�g
				float weight = bone->mWeights[j].mWeight;
				// ���̒��_�̉e�����󂯂�{�[�����X�g�ɁA�{�[���ƃE�F�C�g�̃y�A��ǉ�
				weightLists[vertexIndex].emplace_back(WeightSet{ (UINT)i,weight });
			}
		}

		// �E�F�C�g�̐���
		auto& vertices = model->mesh.vertices;
		// �e���_�ɂ��ď���
		for (uint32_t i = 0; i < mesh->mNumBones; i++)
		{
			// ���_�̃E�F�C�g����ł��傫��4��I��
			auto& weightList = weightLists[i];
			// �召��r�p�̃����_�����w�肵�č~���Ƀ\�[�g
			weightList.sort(
				[](auto const& lhs, auto const& rhs)
				{
					return lhs.weight > rhs.weight;
				});

			int weightArrayIndex = 0;
			// �~���\�[�g�ς݂̃E�F�C�g���X�g����
			for (auto& weightSet : weightList)
			{
				// ���_�f�[�^�ɏ�������
				vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
				vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;
				// 4�ɒB������I��
				if (++weightArrayIndex >= maxBoneIndices)
				{
					float weight = 0.f;
					// 2�Ԗڈȍ~�̃E�F�C�g�����v
					for (size_t j = 1; j < maxBoneIndices; j++)
					{
						weight += vertices[i].boneWeight[j];
					}
					// ���v��1,f(100%)�ɂȂ�悤�ɒ���
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

		// �}�e���A����
		aiString materialName;
		if (material->Get(AI_MATKEY_NAME, materialName) == AI_SUCCESS)
		{
			model->material.name = materialName.C_Str();
		}

		// �A���r�G���g�J���[
		aiColor3D ambientColor;
		if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS)
		{
			model->material.ambient.r = ambientColor.r;
			model->material.ambient.g = ambientColor.g;
			model->material.ambient.b = ambientColor.b;
		}

		// �f�B�t���[�Y�J���[
		aiColor3D diffuseColor;
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
		{
			model->material.diffuse.r = diffuseColor.r;
			model->material.diffuse.g = diffuseColor.g;
			model->material.diffuse.b = diffuseColor.b;
		}

		// �e�N�X�`���[
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

				// �e�N�X�`���ǂݍ���
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
	// ��؂蕶�� '\\' ���o�Ă����ԍŌ�̕���������
	pos1 = path.rfind('\\');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	// ��؂蕶�� '/' ���o�Ă����ԍŌ�̕���������
	pos1 = path.rfind('/');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}
