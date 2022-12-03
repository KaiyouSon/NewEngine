#include "ComponentManager.h"
using namespace std;

ComponentManager::ComponentManager()
{
	// デフォルトで持っているコンポネント
	list.push_back(move(make_unique<Tag>()));			// タグ
	list.push_back(move(make_unique<Transform>()));		// トランスフォーム
	list.push_back(move(make_unique<Blend>()));			// ブレンド
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::AddComponent(unique_ptr<IComponent>&& component)
{
	list.push_back(move(component));
}

void ComponentManager::DestroyComponent(unique_ptr<IComponent>&& component)
{
	for (const auto& temp : list)
	{
		if (temp == component)
		{
			list.remove(temp);
		}
	}
}
