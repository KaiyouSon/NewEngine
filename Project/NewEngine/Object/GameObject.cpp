#include "GameObject.h"

GameObject::GameObject() :
	pos(0), scale(1), rot(0), offset(0), tiling(1), isActive(true), mParent(nullptr),
	mComponentManager(std::make_unique<ComponentManager>(this))
{
	blendMode = BlendMode::Alpha;
}

void GameObject::InitToObject3D()
{
	layerTag = "Object3D";
	mType = GameObjectType::Object3D;

	mComponentManager->AddComponent<Object3DInfo>();
	mComponentManager->AddComponent<Transform>();
	mComponentManager->AddComponent<ModelData>();
	mComponentManager->AddComponent<TextureData>();

	mTransform = mComponentManager->GetComponent<Transform>();
	mTextureData = mComponentManager->GetComponent<TextureData>();
}
void GameObject::InitToParticleMesh()
{
	layerTag = "Object3D";
	mType = GameObjectType::ParticleMesh;

	mComponentManager->AddComponent<ParticleMeshInfo>();
	mComponentManager->AddComponent<Transform>();
	mComponentManager->AddComponent<TextureData>();

	mTransform = mComponentManager->GetComponent<Transform>();
	mTextureData = mComponentManager->GetComponent<TextureData>();
}
void GameObject::InitToSprite()
{
	layerTag = "BackSprite";
	mType = GameObjectType::Sprite;

	mComponentManager->AddComponent<SpriteInfo>();
	mComponentManager->AddComponent<Transform>();
	mComponentManager->AddComponent<TextureData>();
	mComponentManager->AddComponent<ScriptsComponent>();

	mTransform = mComponentManager->GetComponent<Transform>();
	mTextureData = mComponentManager->GetComponent<TextureData>();
}
void GameObject::InitToCamera()
{
	mType = GameObjectType::Camera;

	mComponentManager->AddComponent<CameraInfo>();
	mComponentManager->AddComponent<Transform>();

	mTransform = mComponentManager->GetComponent<Transform>();
}

void GameObject::BaseUpdate()
{
	if (mTransform)
	{
		mTransform->pos = pos;
		mTransform->scale = scale;
		mTransform->rot = rot;
		mTransform->parent = mParent;
	}

	mComponentManager->Update();
}

nlohmann::json GameObject::SaveToJson()
{
	nlohmann::json objData = mComponentManager->SaveToJson();
	objData["type"] = (uint32_t)mType;
	objData["name"] = name;

	return objData;
}
void GameObject::LoadToJson(nlohmann::json& objectField)
{
	nlohmann::json componentField = objectField["components"];
	mComponentManager->LoadToJson(componentField);
}

// ゲッター
Transform* GameObject::GetTransform()
{
	return mTransform;
}
Transform* GameObject::GetParent()
{
	return mParent;
}
GameObjectType GameObject::GetType()
{
	return mType;
}
ComponentManager* GameObject::GetComponentManager()
{
	return mComponentManager.get();
}

// セッター
void GameObject::SetParent(Transform* parent)
{
	mParent = parent;
}
