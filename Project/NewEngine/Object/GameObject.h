#pragma once
#include "Util.h"
#include "Material.h"
#include "StandardLib.h"
#include "ComponentManager.h"

class GameObject
{
protected:
	Transform* mParent;
	GameObjectType mType;

protected:
	// コンポーネント関連
	std::unique_ptr<ComponentManager> mComponentManager;

	Transform* mTransform;
	TextureData* mTextureData;
	GraphicsPipeline* mGraphicsPipelineTest;

public:
	std::string name;
	Vec3 pos;
	Vec3 rot;
	Vec3 scale;
	Color color;
	Vec2 tiling;
	Vec2 offset;
	BlendMode blendMode;
	std::string layerTag;
	bool isActive;

protected:
	void BaseUpdate();

public:
	nlohmann::json SaveToJson();
	void LoadToJson(nlohmann::json& objectField);

public:
	GameObject();
	virtual ~GameObject() {}
	virtual void Update() = 0;
	virtual void ExecuteCS() = 0;
	virtual void AppedToRenderer() = 0;
	virtual void Draw(const std::string& _layerTag = "", const BlendMode _blendMode = BlendMode::Alpha) = 0;

public:
	virtual void Copy(GameObject* gameObj) = 0;
	virtual void SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize = true) = 0;

public:
	// ゲッター
	Transform* GetTransform();
	Transform* GetParent();
	GameObjectType GetType();
	ComponentManager* GetComponentManager();

public:
	// セッター
	void SetParent(Transform* parent);
};

