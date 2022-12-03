#include "ColliderManager.h"
using namespace std;

ColliderManager::ColliderManager() : index(0)
{
}

void ColliderManager::Add(unique_ptr<Collider>&& collider)
{
	list.push_back(move(collider));
	list.back()->SetColliderIndex(index);
	index++;
}

void ColliderManager::DestroyCollider(const Collider& collider)
{
	for (const auto& temp : list)
	{
		if (temp.get() == &collider)
		{
			list.remove(temp);
			break;
		}
	}
}
