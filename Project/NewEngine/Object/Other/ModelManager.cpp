#include "ModelManager.h"
#include "TextureManager.h"
#include "AssimpLoader.h"
#include <fstream>
#include <sstream>

using namespace VertexBufferData;

// ���f���̃}�b�v
std::unordered_map<std::string, std::unique_ptr<Model>> ModelManager::sModelMap;
std::mutex ModelManager::sMtx = std::mutex{};
std::string ModelManager::sDirectoryPath = "Application/Resources/Model/";

// ���f���̎擾
Model* ModelManager::GetModel(const std::string modelTag)
{
	std::string log;
	if (sModelMap[modelTag].get() == nullptr)
	{
		log = "[Model Use] ModelTag : " + modelTag + ", does not exist";
	}
	else
	{
		log = "[Model Use] ModelTag : " + modelTag + ", was used";
	}
	OutputDebugLog(log.c_str());

	return sModelMap[modelTag].get();
}

// obj�t�@�C�����烂�f�������[�h���}�b�v�̊i�[����
Model* ModelManager::LoadObjModel(const std::string fileName, const std::string modelTag, const bool isSmoothing)
{
	// �r������
	std::lock_guard<std::mutex> lock(sMtx);

	// �C���X�^���X����
	std::unique_ptr<Model> model = std::make_unique<ObjModel>();
	model->name = fileName;

	std::string objfile = fileName + ".obj";
	size_t pos;
	// ��؂蕶�� '/' ���o�Ă����ԍŌ�̕���������
	pos = fileName.rfind('/');
	if (pos < fileName.size())
	{
		objfile = fileName.substr(pos + 1, fileName.size() - pos - 1) + ".obj";
	}

	std::string path = sDirectoryPath + fileName + "/";

	// �t�@�C���X�g���[��
	std::ifstream file;
	// .obj�t�@�C�����J��
	file.open(path + objfile);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		std::string log = "[ObjModel Load] FileName : " + fileName + ", Tag : " + modelTag + ", is,failed to load";
		OutputDebugLog(log.c_str());

		assert(0 && "���f���̓ǂݍ��݂����s���܂���");
	}

	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> texcoords;

	// 1�s���ǂݍ���
	std::string line;
	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream lineStream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		std::getline(lineStream, key, ' ');

		if (key == "mtllib")
		{
			// �}�e���A���̃t�@�C������ǂݍ���
			std::string fileName;
			lineStream >> fileName;

			LoadMaterialColor(path + fileName, model.get());
			continue;
		}

		// �擪������v�Ȃ璸�_���W
		if (key == "v")
		{
			// X,Y,Z���W�ǂݍ���
			Vec3 pos{};
			lineStream >> pos.x;
			lineStream >> pos.y;
			lineStream >> pos.z;
			pos.z *= -1;
			// ���W�f�[�^�ɒǉ�
			positions.emplace_back(pos);
		}

		// �擪������vt�Ȃ�e�N�X�`��
		if (key == "vt")
		{
			// uv�����ǂݍ���
			Vec2 texcoord{};
			lineStream >> texcoord.x;
			lineStream >> texcoord.y;
			// v�������]
			texcoord.y = 1.0f - texcoord.y;
			// �e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		// �擪������vn�Ȃ�@���x�N�g��
		if (key == "vn")
		{
			// xyz�����ǂݍ���
			Vec3 normal{};
			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;
			normal.z *= -1;
			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		// �擪������f�Ȃ�|���S��(�O�p�`)
		if (key == "f")
		{
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string indexString;
			int count = 0;

			while (getline(lineStream, indexString, ' '))
			{
				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream indexStream(indexString);
				unsigned short indexPos, indexNormal, indexTexcoord;

				indexStream >> indexPos;
				indexStream.seekg(1, std::ios_base::cur);	// �X���b�V�����΂�
				indexStream >> indexTexcoord;
				indexStream.seekg(1, std::ios_base::cur);	// �X���b�V�����΂�

				indexStream >> indexNormal;
				// ���_�f�[�^�̒ǉ�
				VFbxModel vertex{};
				vertex.pos = positions[indexPos - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];

				model->mesh.AddVertex(vertex);

				// ���_�C���f�b�N�X�ɒǉ�
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

	// �t�@�C�������
	file.close();

	if (isSmoothing == true)
	{
		model->mesh.CalculateSmoothedVertexNormals();
	}

	model->mesh.CreateBuffer();

	std::string log = "[ObjModel Load] FileName : " + fileName + ", Tag : " + modelTag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	// map�Ɋi�[
	sModelMap.insert(std::make_pair(modelTag, std::move(model)));

	return sModelMap[modelTag].get();
}

// fbx�t�@�C�����烂�f�������[�h���}�b�v�̊i�[����
Model* ModelManager::LoadFbxModel(const std::string fileName, const std::string modelTag)
{
	// �r������
	std::lock_guard<std::mutex> lock(sMtx);

	// ���f������
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();
	model->name = fileName;

	// ���f���Ɠ������O�̃t�H���_�[����ǂݍ���
	std::string path = sDirectoryPath + fileName + "/";
	std::string fbxfile = fileName + ".fbx";
	std::string fullPath = path + fbxfile;

	// �t���O
	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;

	// �V�[���̓ǂݍ���
	model->scene = model->importer.ReadFile(fullPath, flags);

	if (model->scene == nullptr)
	{
		std::string log = "[ObjModel Load] FileName : " + fileName + ", Tag : " + modelTag + ", is,failed to load";
		OutputDebugLog(log.c_str());

		assert(0 && "���f���̓ǂݍ��݂����s���܂���");
	}

	// �}�e���A���̉��
	AssimpLoader::GetInstance()->ParseMaterial(model.get(), model->scene);
	AssimpLoader::GetInstance()->ParseNodeRecursive(model.get(), nullptr, model->scene->mRootNode);

	// �o�b�t�@�[����
	model->mesh.vertexBuffer.Create(model->mesh.vertices);
	model->mesh.indexBuffer.Create(model->mesh.indices);

	// map�Ɋi�[
	sModelMap.insert(std::make_pair(modelTag, std::move(model)));


	std::string log = "[FbxModel Load] FileName : " + fileName + ", Tag : " + modelTag + ", was loaded successfully";
	OutputDebugLog(log.c_str());

	return sModelMap[modelTag].get();
}

// .mtl�t�@�C���̓ǂݍ���
void ModelManager::LoadMaterialColor(std::string filePath, Model* model)
{
	// �t�@�C���X�g���[��
	std::ifstream file;
	// .mtl�t�@�C�����J��
	file.open(filePath);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0 && "�}�e���A���̓ǂݍ��݂����s���܂���");
	}

	// �f�B���N�g���p�X
	std::string directoryPath = filePath;
	while (true)
	{
		directoryPath.pop_back();
		if (directoryPath.back() == '/')
		{
			break;
		}
	}

	// 1�s���ǂݍ���
	std::string line;
	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream lineStream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		std::getline(lineStream, key, ' ');

		// �擪�̃^�u�����𖳎�����
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		// �擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl")
		{
			// �}�e���A�����ǂݍ���
			lineStream >> model->material.name;
		}

		// �擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			lineStream >> model->material.ambient.r;
			lineStream >> model->material.ambient.g;
			lineStream >> model->material.ambient.b;
		}

		// �擪������Ka�Ȃ�f�B�t�[�Y�F
		if (key == "Kd")
		{
			lineStream >> model->material.diffuse.r;
			lineStream >> model->material.diffuse.g;
			lineStream >> model->material.diffuse.b;
		}

		// �擪������Ka�Ȃ�X�y�L�����[�F
		if (key == "Ks")
		{
			lineStream >> model->material.specular.r;
			lineStream >> model->material.specular.g;
			lineStream >> model->material.specular.b;
		}

		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			std::string textureName;
			lineStream >> textureName;

			std::string textureTag = model->name + "Texture";

			// �e�N�X�`���ǂݍ���
			model->texture = TextureManager::LoadMaterialTexture(directoryPath + textureName, textureTag);
		}
	}

	// �t�@�C�������
	file.close();
}
