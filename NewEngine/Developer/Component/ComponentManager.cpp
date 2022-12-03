#include "ComponentManager.h"
using namespace std;

ComponentManager::ComponentManager()
{
	// �f�t�H���g�Ŏ����Ă���R���|�l���g
	list.push_back(move(make_unique<Tag>()));			// �^�O
	list.push_back(move(make_unique<Transform>()));		// �g�����X�t�H�[��
	list.push_back(move(make_unique<Blend>()));			// �u�����h
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
