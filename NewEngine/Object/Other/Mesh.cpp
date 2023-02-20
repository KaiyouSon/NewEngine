#include "Mesh.h"

void Mesh::AddSmoothData(const unsigned short& indexPos, const unsigned short& indexVertex)
{
	smoothData[indexPos].emplace_back(indexVertex);
}

void Mesh::CalculateSmoothedVertexNormals()
{
	for (auto& currentData : smoothData)
	{
		// �e�ʗp�̋��ʒ��_�R���N�V����
		std::vector<unsigned short>& v = currentData.second;

		// �S���_�̖@���𕽋ς���
		Vec3 normal;
		for (unsigned short& index : v)
		{
			normal += {	vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z};
		}
		normal = (normal / (float)v.size()).Norm();

		// ���ʖ@�����g�p���邷�ׂĂ̒��_�f�[�^�ɏ�������
		for (unsigned short& index : v)
		{
			vertices[index].normal = { normal.x,normal.y,normal.z };
		}
	}
}

void Mesh::AddVertex(const VertexPosNormalUvBone& vertex)
{
	vertices.emplace_back(vertex);
}

void Mesh::AddIndex(const unsigned short& index)
{
	indices.emplace_back(index);
}

void Mesh::CreateBuffer()
{
	vertexBuffer.Create(vertices);
	indexBuffer.Create(indices);
}
