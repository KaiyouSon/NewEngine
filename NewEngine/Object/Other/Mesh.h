#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <unordered_map>

class Mesh
{
private:
	// 頂点法線スムージング用データ
	std::unordered_map<uint16_t, std::vector<uint16_t>> smoothData;

public:
	std::vector<VertexBufferData::VFbxModel> vertices;
	VertexBuffer<VertexBufferData::VFbxModel> vertexBuffer;
	std::vector<uint16_t> indices;
	IndexBuffer indexBuffer;

public:
	// スムージングデータの追加
	void AddSmoothData(const uint16_t indexPos, const uint16_t indexVertex);

	// Vertexの追加
	void AddVertex(const VertexBufferData::VFbxModel vertex);

	// Indexの追加
	void AddIndex(const uint16_t index);

	// バッファーの生成
	void CreateBuffer();

	// 平滑化された頂点法線の計算
	void CalculateSmoothedVertexNormals();
};

