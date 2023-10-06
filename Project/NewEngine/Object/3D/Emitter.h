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

class Emitter
{
private:
	std::vector<VertexBufferData::VParticle> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VParticle>> mVertexBuffer;

	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	Texture* mTexture;
	Material mMaterial;
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

public: //繧ｻ繝・ち繝ｼ

	// 繝・け繧ｹ繝√Ε繝ｼ
	void SetTexture(Texture* texture);

	// 繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繝代う繝励Λ繧､繝ｳ
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ謨ｰ
	void SetMaxParticle(const uint32_t max);

public: // 繧ｲ繝・ち繝ｼ

	// 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・
	Vec3 GetWorldPos();

	// 繝ｯ繝ｼ繝ｫ繝峨せ繧ｱ繝ｼ繝ｫ
	Vec3 GetWorldScale();

	// 繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝
	Transform GetTransform();

};
