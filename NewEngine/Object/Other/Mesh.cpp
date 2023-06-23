#include "Mesh.h"

void Mesh::AddSmoothData(const uint16_t indexPos, const uint16_t indexVertex)
{
	smoothData[indexPos].emplace_back(indexVertex);
}

void Mesh::CalculateSmoothedVertexNormals()
{
	for (auto& currentData : smoothData)
	{
		// 各面用の共通頂点コレクション
		std::vector<uint16_t> v = currentData.second;

		// 全頂点の法線を平均する
		Vec3 normal;
		for (uint16_t index : v)
		{
			normal += {	vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z};
		}
		normal = (normal / (float)v.size()).Norm();

		// 共通法線を使用するすべての頂点データに書き込む
		for (uint16_t index : v)
		{
			vertices[index].normal = { normal.x,normal.y,normal.z };
		}
	}
}

void Mesh::AddVertex(const VertexBufferData::VFbxModel vertex)
{
	vertices.emplace_back(vertex);
}

void Mesh::AddIndex(const uint16_t index)
{
	indices.emplace_back(index);
}

void Mesh::CreateBuffer()
{
	vertexBuffer.Create(vertices);
	indexBuffer.Create(indices);
}
