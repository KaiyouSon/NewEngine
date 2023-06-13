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
	VertexBuffer<VertexPosNormalUvBone> vertexBuffer;
	IndexBuffer indexBuffer;
	std::vector<VertexPosNormalUvBone> vertices;
	std::vector<uint16_t> indices;

public:

	// スムージングデータの追加
	void AddSmoothData(const uint16_t indexPos, const uint16_t indexVertex);

	// Vertexの追加
	void AddVertex(const VertexPosNormalUvBone vertex);

	// Indexの追加
	void AddIndex(const uint16_t index);

	// バッファーの生成
	void CreateBuffer();

	// 平滑化された頂点法線の計算
	void CalculateSmoothedVertexNormals();

public:
	// ゲッター
	inline size_t GetVertexSize() { return vertices.size(); }
	inline size_t GetIndexSize() { return indices.size(); }

	inline VertexBuffer<VertexPosNormalUvBone> GetVertexBuffer() { return vertexBuffer; }
	inline IndexBuffer GetIndexBuffer() { return indexBuffer; }
};

