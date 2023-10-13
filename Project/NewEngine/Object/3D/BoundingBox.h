#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "ParticleParam.h"
#include "StructuredBuffer.h"

class BoundingBox
{
private:
	std::vector<VertexBufferData::VBoundingBox> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VBoundingBox>> mVertexBuffer;
	std::vector<uint16_t> mIndices;
	std::unique_ptr<IndexBuffer> mIndexBuffer;
	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	ITexture* mTexture;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Vec3 tiling;
	Vec3 offset;
	std::vector<ParticleParameter::PParam0> pParam;
	uint32_t pSize;

private:
	void VertexDataInit();

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	BoundingBox();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public: // セッター
	void SetTexture(ITexture* texture);
};

