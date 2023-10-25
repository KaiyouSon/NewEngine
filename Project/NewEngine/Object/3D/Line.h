#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include <vector>
#include <memory>

// 線のクラス
class Line
{
private:
	std::vector<VertexBufferData::VLine> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VLine>> mVertexBuffer;
	std::unique_ptr<Material> mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Transform mTransform;
	Transform* mParent;

public:
	Vec3 startPos;
	Vec3 endPos;
	Vec3 pos;
	Vec3 rot;
	Color color;

private: // マテリアル関連
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	Line();
	void Update(Transform* parent);
	void Draw();
};

