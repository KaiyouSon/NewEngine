#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "Camera.h"

class Object3D
{
private:
	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	Model* mModel;
	Texture* mTexture;
	Texture* mDissolveTex;
	Texture* mDepthTex;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Camera* mCamera;
	bool mIsWriteShadow;
	bool mIsWriteDepth;

private:
	// 蠕梧紛逅・☆繧・
	Texture* mWhiteTex;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Vec2 tiling;
	Vec2 offset;

	float dissolve;
	float colorPower;
	Color dissolveColor;

	bool isLighting;
	bool isUseDissolve;
	static bool isAllLighting;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	Object3D();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public: //繧ｻ繝・ち繝ｼ

	// 繝｢繝・Ν
	void SetModel(Model* model);

	// 繝・け繧ｹ繝√Ε繝ｼ
	void SetTexture(Texture* texture);

	// 繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繝代う繝励Λ繧､繝ｳ
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ
	void SetAnimation(const uint32_t animationIndex, const uint32_t maxFrame, const bool isPlay = true);

	// 繧ｫ繝｡繝ｩ
	void SetCamera(Camera* camera = nullptr);

	// 蠖ｱ
	void SetisShadow(const bool isWriteShadow, const bool isWriteDepth);

	// 隕ｪ
	void SetParent(Transform* parent);

	// 繝薙Ν繝懊・繝・
	void SetBillboardType(const BillboardType type);

public: // 繧ｲ繝・ち繝ｼ

	// 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・
	Vec3 GetWorldPos();

	// 繝ｯ繝ｼ繝ｫ繝峨せ繧ｱ繝ｼ繝ｫ
	Vec3 GetWorldScale();

	// 繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝
	Transform GetTransform();

	// 隕ｪ縺ｮ繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝
	Transform* GetParent();

	// 繝｢繝・Ν
	Model* GetModel();
};

