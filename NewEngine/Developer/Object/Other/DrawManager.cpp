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

	// �t�@�C���X�g���[��
	ifstream file;
	// .obj�t�@�C�����J��
	file.open(filePath);
	// �t�@�C���I�[�v�����s���`�F�b�N
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

	// 1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		istringstream lineStream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(lineStream, key, ' ');

		if (key == "mtllib")
		{
			// �}�e���A���̃t�@�C������ǂݍ���
			string filepath;
			lineStream >> filepath;

			if (filepath.size() > 0)
			{
				materialList->AddMaterial(LoadMaterial(materialFileName + filepath), numberIndex);
				numberIndex++;
			}
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
			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		// �擪������f�Ȃ�|���S��(�O�p�`)
		if (key == "f")
		{
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string indexString;
			while (getline(lineStream, indexString, ' '))
			{
				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				istringstream indexStream(indexString);
				unsigned short indexPos, indexNormal, indexTexcoord;
				indexStream >> indexPos;
				indexStream.seekg(1, ios_base::cur);	// �X���b�V�����΂�
				indexStream >> indexTexcoord;
				indexStream.seekg(1, ios_base::cur);	// �X���b�V�����΂�
				indexStream >> indexNormal;
				// ���_�f�[�^�̒ǉ�
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPos - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				modelData->vertices.emplace_back(vertex);

				// ���_�C���f�b�N�X�ɒǉ�
				modelData->indices.emplace_back((unsigned short)modelData->indices.size());
			}
		}
	}

	// �t�@�C�������
	file.close();

	return move(modelData);
}

unique_ptr<Material> LoadMaterial(const std::string& filePath)
{
	static uint32_t numberIndex = 0;
	unique_ptr<Material> material = move(make_unique<Material>());

	// �t�@�C���X�g���[��
	ifstream file;
	// .obj�t�@�C�����J��
	file.open(filePath);
	// �t�@�C���I�[�v�����s���`�F�b�N
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

	// 1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		istringstream lineStream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(lineStream, key, ' ');

		// �擪�̃^�u�����𖳎�����
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		// �擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl")
		{
			// �}�e���A�����ǂݍ���
			lineStream >> material->name;
		}

		// �擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			lineStream >> material->ambient.x;
			lineStream >> material->ambient.y;
			lineStream >> material->ambient.z;
		}

		// �擪������Ka�Ȃ�f�B�t�[�Y�F
		if (key == "Kd")
		{
			lineStream >> material->diffuse.x;
			lineStream >> material->diffuse.y;
			lineStream >> material->diffuse.z;
		}

		// �擪������Ka�Ȃ�X�y�L�����[�F
		if (key == "Kd")
		{
			lineStream >> material->specular.x;
			lineStream >> material->specular.y;
			lineStream >> material->specular.z;
		}

		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			lineStream >> material->textureFilepath;

			// �e�N�X�`���ǂݍ���
			//materialTextureList->Add(LoadTexture((directoryPath + material->textureFilepath).c_str()), numberIndex);
			numberIndex++;
		}
	}

	// �t�@�C�������
	file.close();

	return move(material);
}

void UnLoadTexture(Texture* texture)
{
	TextureBuffer::UnLoadTexture(texture);
}