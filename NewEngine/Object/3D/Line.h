#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <vector>
#include <memory>

class Line
{
private:
	std::vector<VertexBufferData::VLine> vertices_;
	std::unique_ptr<VertexBuffer<VertexBufferData::VLine>> vertexBuffer_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CTransform3D>> constantBufferTransform_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CColor>> constantBufferColor_;
	GraphicsPipeline* graphicsPipeline_;
	Transform transform_;

public:
	Vec3 pos;
	float scale;
	Vec3 rot;
	Color color;

public:
	Line();
	void Update();
	void Draw();
};

