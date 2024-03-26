#include "Mesh.h"

void Mesh::AddSmoothData(const uint16_t indexPos, const uint16_t indexVertex)
{
	smoothData[indexPos].emplace_back(indexVertex);
}

void Mesh::CalculateSmoothedVertexNormals()
{
    for (auto& currentData : smoothData)
    {
        // 各頂点の法線データを格納したベクトル
        std::vector<uint16_t> v = currentData.second;

        // 平滑化された頂点法線を計算するためのベクトル
        Vec3 normal;
        for (uint16_t index : v)
        {
            normal += {vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z};
        }
        normal = (normal / static_cast<float>(v.size())).Norm();

        // 平滑化された法線データを頂点に設定し、頂点データを更新
        for (uint16_t index : v)
        {
            vertices[index].normal = { normal.x, normal.y, normal.z };
        }
    }
}

void Mesh::AddVertex(const VertexBufferData::VFbxModel vertex)
{
	vertices.emplace_back(vertex);
}

void Mesh::AddIndex(const uint32_t index)
{
	indices.emplace_back(index);
}

void Mesh::CreateBuffer()
{
	vertexBuffer.Create(vertices);
	indexBuffer.Create(indices);
}

