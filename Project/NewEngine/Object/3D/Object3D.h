#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "Camera.h"

// オブジェクトのクラス
class Object3D
{
private:
	std::unique_ptr<Material> mMaterial;
	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	Model* mModel;
	Texture* mTexture;
	Texture* mDissolveTex;
	DepthTexture* mDepthTex;
	GraphicsPipeline* mGraphicsPipeline;
	Camera* mCamera;
	bool mIsWriteShadow;
	bool mIsWriteDepth;

private:
	// テクスチャ
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

public: // セッター
	void SetModel(Model* model);
	void SetTexture(Texture* texture);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetAnimation(const uint32_t animationIndex, const uint32_t maxFrame, const bool isPlay = true);
	void SetCamera(Camera* camera = nullptr);
	void SetisShadow(const bool isWriteShadow, const bool isWriteDepth);
	void SetParent(Transform* parent);
	void SetBillboardType(const BillboardType type);

public: //  ゲッター
	Vec3 GetWorldPos();
	Vec3 GetWorldScale();
	Transform GetTransform();
	Transform* GetParent();
	Model* GetModel();
};
