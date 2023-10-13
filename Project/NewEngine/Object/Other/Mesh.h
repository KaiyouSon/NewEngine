#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <unordered_map>

class Mesh
{
private:
	// 鬆らせ豕慕ｷ壹せ繝繝ｼ繧ｸ繝ｳ繧ｰ逕ｨ繝・・繧ｿ
	std::unordered_map<uint16_t, std::vector<uint16_t>> smoothData;

public:
	std::vector<VertexBufferData::VFbxModel> vertices;
	VertexBuffer<VertexBufferData::VFbxModel> vertexBuffer;
	std::vector<uint16_t> indices;
	IndexBuffer indexBuffer;

public:
	// 繧ｹ繝繝ｼ繧ｸ繝ｳ繧ｰ繝・・繧ｿ縺ｮ霑ｽ蜉
	void AddSmoothData(const uint16_t indexPos, const uint16_t indexVertex);

	// Vertex縺ｮ霑ｽ蜉
	void AddVertex(const VertexBufferData::VFbxModel vertex);

	// Index縺ｮ霑ｽ蜉
	void AddIndex(const uint16_t index);

	// 繝舌ャ繝輔ぃ繝ｼ縺ｮ逕滓・
	void CreateBuffer();

	// 蟷ｳ貊大喧縺輔ｌ縺滄らせ豕慕ｷ壹・險育ｮ・
	void CalculateSmoothedVertexNormals();
};
