#include "EmptyObject.h"

EmptyObject::EmptyObject()
{
}

EmptyObject::EmptyObject(const std::string& name)
{
	this->name = name;

	InitComponents();
}

void EmptyObject::InitComponents()
{
	mType = GameObjectType::EmptyObject;

	mComponentManager->AddComponent<EmptyObjectInfo>();
	mComponentManager->AddComponent<Transform>();
	mComponentManager->AddComponent<ScriptsComponent>();

	mTransform = mComponentManager->GetComponent<Transform>();
}

void EmptyObject::Update()
{
	if (!isActive)
	{
		return;
	}

	BaseUpdate();
}

void EmptyObject::Copy(GameObject* gameObj)
{
	color = gameObj->color;
	mComponentManager->Copy(gameObj->GetComponentManager());

	for (const auto& child : gameObj->GetChilds())
	{
		GameObject* obj = gCurrentScene->GetGameObjectManager()->CreateGameObject(child);
		obj->SetParent(this);
	}
}