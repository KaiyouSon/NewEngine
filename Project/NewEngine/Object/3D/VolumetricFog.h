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

class VolumetricFog
{
private:
	std::vector<VertexBufferData::VVolumetricFog> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VVolumetricFog>> mVertexBuffer;
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
	Vec3 moveSpeed;
	uint32_t pSize;

	ConstantBufferData::CVolumetricFog fogParam;

private:
	void VertexDataInit();

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	VolumetricFog();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public: // セッター
	void SetTexture(ITexture* texture);
};

