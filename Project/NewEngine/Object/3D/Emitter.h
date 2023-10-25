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

// エミッターのクラス
class Emitter
{
private:
	std::vector<VertexBufferData::VParticle> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VParticle>> mVertexBuffer;
	std::unique_ptr<Material> mMaterial;
	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	Texture* mTexture;
	
	GraphicsPipeline* mGraphicsPipeline;
	Billboard mBillboard;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Vec2 tiling;
	Vec2 offset;
	std::vector<ParticleParameter::PParam0> pParam;
	uint32_t pSize;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	Emitter();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public: // セッター
	void SetTexture(Texture* texture);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetMaxParticle(const uint32_t max);

public: // ゲッター
	Vec3 GetWorldPos();
	Vec3 GetWorldScale();
	Transform GetTransform();
};
