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

		if (mParent)
		{
			mTransform->parent = mParent->mTransform;
		}
		else
		{
			mTransform->parent = nullptr;
		}
	}

	mComponentManager->Update();
}

nlohmann::json GameObject::SaveToJson()
{
	nlohmann::json objData = mComponentManager->SaveToJson();
	objData["type"] = (uint32_t)mType;
	objData["name"] = name;
	objData["parent_tag"] = mParentTag;

	return objData;
}
void GameObject::LoadToJson(nlohmann::json& objectField)
{
	nlohmann::json componentField = objectField["components"];
	mComponentManager->LoadToJson(componentField);

	mParentTag = objectField["parent_tag"];
}

bool GameObject::CheckActive()
{
	if (!isActive)
	{
		return false;
	}

	// 親が存在する場合
	if (mParent)
	{
		if (!mParent->CheckActive())
		{
			return false;
		}
	}

	return true;
}

// ゲッター
Transform* GameObject::GetTransform()
{
	return mTransform;
}
void GameObject::ParentChildCancel()
{
	if (!mParent)
	{
		return;
	}

	for (uint32_t i = 0; i < mParent->mChilds.size(); i++)
	{
		GameObject* child = mParent->mChilds[i];
		if (child == this)
		{
			mParent->mChilds.erase(mChilds.begin() + i);
			break;
		}
	}

	mParent = nullptr;
}
ScriptsComponent* GameObject::GetScriptsComponent()
{
	return mScriptsComponent;
}
GameObjectType GameObject::GetType()
{
	return mType;
}
ComponentManager* GameObject::GetComponentManager()
{
	return mComponentManager.get();
}
GameObject* GameObject::GetParent()
{
	return mParent;
}
std::vector<GameObject*> GameObject::GetChilds()
{
	return mChilds;
}

// セッター
void GameObject::SetParent(Transform* parent)
{
	parent;
}
void GameObject::SetParent(GameObject* parent)
{
	if (parent)
	{
		parent->mChilds.push_back(this);
		mParentTag = parent->name;
	}

	mParent = parent;
}
void GameObject::SetChild(GameObject* child)
{
	if (!child)
	{
		return;
	}

	child->SetParent(this);
}