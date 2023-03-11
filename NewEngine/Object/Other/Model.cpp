#include "Model.h"
#include <fstream>
#include <sstream>
using namespace std;

Model::Model(const std::string& modelName, const bool& isSmoothing)
{
	string path = "Application/Resources/Model/" + modelName + "/";
	string objfile = modelName + ".obj";

	// �t�@�C���X�g���[��
	ifstream file;
	// .obj�t�@�C�����J��
	file.open(path + objfile);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0 && "���f���̓ǂݍ��݂����s���܂���");
	}

	vector<Vec3> positions;
	vector<Vec3> normals;
	vector<Vec2> texcoords;

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
			string fileName;
			lineStream >> fileName;

			material = Material(path + fileName);
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
			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		// �擪������f�Ȃ�|���S��(�O�p�`)
		if (key == "f")
		{
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string indexString;
			int count = 0;

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
				VertexPosNormalUvBone vertex{};
				vertex.pos = positions[indexPos - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];

				mesh.AddVertex(vertex);

				if (isSmoothing == true)
				{
					mesh.AddSmoothData(indexPos, (unsigned short)mesh.GetVertexSize() - 1);
				}

				// ���_�C���f�b�N�X�ɒǉ�
				if (count % 3 == 0)
				{
					mesh.AddIndex((unsigned short)mesh.GetIndexSize());
				}
				if (count % 3 == 1)
				{
					mesh.AddIndex((unsigned short)mesh.GetIndexSize() + 1);
				}
				if (count % 3 == 2)
				{
					mesh.AddIndex((unsigned short)mesh.GetIndexSize() - 1);
				}

				count++;
			}
		}
	}

	// �t�@�C�������
	file.close();

	if (isSmoothing == true)
	{
		mesh.CalculateSmoothedVertexNormals();
	}

	mesh.CreateBuffer();
}