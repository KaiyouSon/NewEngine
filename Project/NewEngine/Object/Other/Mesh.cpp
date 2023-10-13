#include "Mesh.h"

void Mesh::AddSmoothData(const uint16_t indexPos, const uint16_t indexVertex)
{
	smoothData[indexPos].emplace_back(indexVertex);
}

void Mesh::CalculateSmoothedVertexNormals()
{
	for (auto& currentData : smoothData)
	{
		// 蜷・擇逕ｨ縺ｮ蜈ｱ騾夐らせ繧ｳ繝ｬ繧ｯ繧ｷ繝ｧ繝ｳ
		std::vector<uint16_t> v = currentData.second;

		// 蜈ｨ鬆らせ縺ｮ豕慕ｷ壹ｒ蟷ｳ蝮・☆繧・
		Vec3 normal;
		for (uint16_t index : v)
		{
			normal += {	vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z};
		}
		normal = (normal / (float)v.size()).Norm();

		// 蜈ｱ騾壽ｳ慕ｷ壹ｒ菴ｿ逕ｨ縺吶ｋ縺吶∋縺ｦ縺ｮ鬆らせ繝・・繧ｿ縺ｫ譖ｸ縺崎ｾｼ繧
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

