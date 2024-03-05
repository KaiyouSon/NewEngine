#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "Camera.h"

#include "GameObject.h"

// オブジェクトのクラス
class Object3D : public GameObject
{
private:
	std::unique_ptr<Material> mMaterial;
	Vec3 mWorldPos;
	Vec3 mWorldScale;
	//Model* mModel;
	Texture* mTexture;
	Texture* mDissolveTex;
	DepthTexture* mDepthTex;
	GraphicsPipeline* mGraphicsPipeline;
	Camera* mCamera;
	bool mIsWriteShadow;
	bool mIsWriteDepth;

private:
	// コンポーネント関連
	ModelData* mModelData;

private:
	// テクスチャ
	Texture* mWhiteTex;

public:
	float dissolve;
	float colorPower;
	Color dissolveColor;

	bool isLighting;
	bool isUseDissolve;
	static bool isAllLighting;
	static float sShadowBias;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

private:
	void DrawCommands();

public:
	Object3D();
	Object3D(const std::string& name);
	void Update() override;
	void ExecuteCS() override;
	void AppedToRenderer() override;
	void Draw(const std::string& _layerTag = "", const BlendMode blendMode = BlendMode::Alpha) override;
	void Copy(GameObject* gameObj) override;

public: // セッター
	void SetModel(Model* model);
	void SetModel(const std::string& tag);
	void SetTexture(Texture* texture);
	void SetTexture(const std::string& texTag, [[maybe_unused]] const bool isChangeSize = true) override;
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetAnimation(const uint32_t animationIndex, const uint32_t maxFrame, const bool isPlay = true);
	void SetCamera(Camera* camera = nullptr);
	void SetisShadow(const bool isWriteShadow, const bool isWriteDepth);
	void SetBillboardType(const BillboardType type);

public: //  ゲッター
	Vec3 GetWorldPos();
	Vec3 GetWorldScale();
	Model* GetModel();
};
