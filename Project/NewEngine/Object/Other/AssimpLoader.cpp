#include "AssimpLoader.h"
#include "TextureManager.h"

using namespace VertexBufferData;

void AssimpLoader::ParseMesh(FbxModel* model, aiMesh* mesh)
{
	ParseMeshVertices(model, mesh);
	ParseMeshFaces(model, mesh);
	ParseSkin(model, mesh);
}
void AssimpLoader::ParseMeshVertices(FbxModel* model, aiMesh* mesh)
{
	std::vector<VFbxModel>& vertex = model->mesh.vertices;
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
		vertex[i].uv.y = -mesh->mTextureCoords[0][i].y;
	}
}
void AssimpLoader::ParseMeshFaces(FbxModel* model, aiMesh* mesh)
{
	std::vector<uint16_t>& indices = model->mesh.indices;
	indices.resize(mesh->mNumFaces * 3);

	// �t�F���X
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices[i * 3 + j] = static_cast<uint16_t>(face.mIndices[j]);
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
			uint32_t index;
			float weight;
		};

		// �񎟌��z��i�W���O�z��j list:���_���e�����󂯂�{�[���̑S���X�g vector:�����S���_��
		std::vector<std::list<WeightSet>> weightLists(model->mesh.vertices.size());

		// �{�[���̍ő吔�ݒ�
		model->bones.resize(mesh->mNumBones);

		// �X�L�j���O���̏���
		for (uint32_t i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* bone = mesh->mBones[i];

			// �{�[���̖��O
			model->bones[i].name = bone->mName.C_Str();

			// �{�[���̏����p���s��(�o�C���h�|�[�Y�s��)
			Mat4 initalMat = ConvertMat4FromAssimpMat(bone->mOffsetMatrix);
			model->bones[i].offsetMat = initalMat.Transpose();

			// �E�F�C�g�̓ǂݎ��
			for (uint32_t j = 0; j < bone->mNumWeights; j++)
			{
				// ���_�ԍ�
				int vertexIndex = bone->mWeights[j].mVertexId;
				// �X�L���E�F�C�g
				float weight = bone->mWeights[j].mWeight;
				// ���̒��_�̉e�����󂯂�{�[�����X�g�ɁA�{�[���ƃE�F�C�g�̃y�A��ǉ�
				weightLists[vertexIndex].emplace_back(WeightSet{ i,weight });
			}
		}

		// �E�F�C�g�̐���
		auto& vertices = model->mesh.vertices;
		// �e���_�ɂ��ď���
		for (uint32_t i = 0; i < vertices.size(); i++)
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
		for (int j = 0; j < textureCount; j++)
		{
			aiString texturePath;

			if (material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath) == AI_SUCCESS)
			{
				const std::string filePath = texturePath.C_Str();

				std::string pathStr(filePath);
				std::string name = ExractFileName(pathStr);
				std::string baseDirectory = "Application/Resources/Model/";
				std::string fullPath = baseDirectory + model->name + "/" + name;
				std::string textureTag = model->name + "Texture";

				// �e�N�X�`���ǂݍ���
				model->texture = TextureManager::LoadMaterialTexture(fullPath, textureTag);
			}
		}
	}
}
void AssimpLoader::ParseNodeRecursive(FbxModel* model, FbxNode* parent, const aiNode* node)
{
	aiString nodeName = node->mName;

	// ���f���Ƀm�[�h��ǉ�
	model->nodes.emplace_back();
	FbxNode& modelNode = model->nodes.back();

	// �m�[�h�����擾
	modelNode.name = node->mName.C_Str();

	// ���[�J���s��
	modelNode.localTransformMat = ConvertMat4FromAssimpMat(node->mTransformation);

	// �O���[�o���s��
	modelNode.globalTransformMat = modelNode.localTransformMat;
	if (parent)
	{
		modelNode.parent = parent;
		// �e�̕ό`����Z
		modelNode.globalTransformMat *= parent->globalTransformMat;
	}

	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aimesh = model->scene->mMeshes[node->mMeshes[i]];
		if (aimesh)
		{
			ParseMesh(model, aimesh);
		}
	}

	// �ċA
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		ParseNodeRecursive(model, &modelNode, node->mChildren[i]);
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
