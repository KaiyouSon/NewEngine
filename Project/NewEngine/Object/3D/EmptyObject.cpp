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
