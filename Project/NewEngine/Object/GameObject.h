#pragma once
#include "Util.h"
#include "Material.h"
#include "StandardLib.h"
#include "ComponentManager.h"

class GameObjectManager;
class GameObjectInfo;

class GameObject
{
private:
	std::string mParentTag;

protected:
	GameObjectType mType;
	GameObject* mParent;
	std::vector<GameObject*> mChilds;

protected:
	// コンポーネント関連
	std::unique_ptr<ComponentManager> mComponentManager;

	Transform* mTransform;
	TextureComponent* mTextureComponent;
	MaterialComponent* mMaterialComponent;
	ScriptsComponent* mScriptsComponent;
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
	bool CheckActive();

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
	void ParentChildCancel();

public:
	// ゲッター
	Transform* GetTransform();
	GameObject* GetParent();
	GameObjectType GetType();
	ComponentManager* GetComponentManager();
	ScriptsComponent* GetScriptsComponent();
	std::vector<GameObject*> GetChilds();

public:
	// セッター
	void SetParent(Transform* parent);
	void SetParent(GameObject* parent);

	void SetChild(GameObject* child);

private:
	friend GameObjectManager;
	friend GameObjectInfo;
};

