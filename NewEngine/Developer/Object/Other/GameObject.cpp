#include "GameObject.h"
using namespace std;

void GameObject::ColliderManagerSetting()
{
	for (const auto& temp : colliderManager->list)
	{
		if (temp->GetColliderType() == SphereShape)
		{
			temp->GetSphereCollider()->centerPos =
				componentManager->GetComponent<Transform>()->pos +
				temp->GetSphereCollider()->posOffSet;
		}
	}
}

GameObject::GameObject() :
	isShow(true), layer(false),
	isShowDataToInspector(false),
	componentManager(move(make_unique<ComponentManager>())),
	colliderManager(move(make_unique<ColliderManager>()))
{
}

GameObject::~GameObject() {}
