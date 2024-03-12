#include "GameObject.h"

GameObject::GameObject() :
	pos(0), scale(1), rot(0), offset(0), tiling(1), isActive(true), mParent(nullptr),
	mComponentManager(std::make_unique<ComponentManager>(this))
{
	blendMode = BlendMode::Alpha;
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
