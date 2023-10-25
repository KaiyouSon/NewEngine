#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <unordered_map>

class Mesh
{
private:
    // 頂点ごとに平滑化された法線データを格納するためのマップ
    std::unordered_map<uint16_t, std::vector<uint16_t>> smoothData;

public:
    std::vector<VertexBufferData::VFbxModel> vertices;
    VertexBuffer<VertexBufferData::VFbxModel> vertexBuffer;
    std::vector<uint16_t> indices;
    IndexBuffer indexBuffer;

public:
    // 平滑化データにインデックス位置と頂点インデックスを追加
    void AddSmoothData(const uint16_t indexPos, const uint16_t indexVertex);

    // 頂点データを追加
    void AddVertex(const VertexBufferData::VFbxModel vertex);

    // インデックスデータを追加
    void AddIndex(const uint16_t index);

    // 頂点バッファとインデックスバッファを作成
    void CreateBuffer();

    // 平滑化された頂点法線を計算
    void CalculateSmoothedVertexNormals();
};