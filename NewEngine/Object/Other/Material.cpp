#include "Material.h"
#include "TextureManager.h"
#include <fstream>
#include <sstream>
#include <cassert>
using namespace std;

Material::Material() :
	ambient(0.3f, 0.3f, 0.3f), diffuse(0.0f, 0.0f, 0.0f),
	specular(0.0f, 0.0f, 0.0f), alpha(1.0f), texture(*TextureManager::GetTexture("White"))
{
}
Material::Material(std::string filePath) :
	ambient(0.3f, 0.3f, 0.3f), diffuse(0.0f, 0.0f, 0.0f),
	specular(0.0f, 0.0f, 0.0f), alpha(1.0f), texture(*TextureManager::GetTexture("White"))
{
	// �t�@�C���X�g���[��
	ifstream file;
	// .obj�t�@�C�����J��
	file.open(filePath);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0 && "�}�e���A���̓ǂݍ��݂����s���܂���");
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
			lineStream >> name;
		}

		// �擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			lineStream >> ambient.x;
			lineStream >> ambient.y;
			lineStream >> ambient.z;
		}

		// �擪������Ka�Ȃ�f�B�t�[�Y�F
		if (key == "Kd")
		{
			lineStream >> diffuse.x;
			lineStream >> diffuse.y;
			lineStream >> diffuse.z;
		}

		// �擪������Ka�Ȃ�X�y�L�����[�F
		if (key == "Ks")
		{
			lineStream >> specular.x;
			lineStream >> specular.y;
			lineStream >> specular.z;
		}

		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			string textureName;
			lineStream >> textureName;

			// �e�N�X�`���ǂݍ���
			texture = TextureManager::LoadMaterialTexture(directoryPath + textureName);
			texture.isMaterial = true;
		}
	}

	// �t�@�C�������
	file.close();
}
