#pragma once
#include "Collider.h"
#include <list>
#include <memory>

class ColliderManager
{
private:
	int index;
public:
	std::list<std::unique_ptr<Collider>> list;

public:
	ColliderManager();
	void Add(std::unique_ptr<Collider>&& collider);
	void DestroyCollider(const Collider& collider);
};

