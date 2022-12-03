#pragma once
#include "DrawManager.h"
#include <vector>
#include <memory>
#include <list>
class ObjectManager
{
public:
	std::list<std::unique_ptr<GameObject>> list;

public:
	void Update();
	void Draw3D();
	void Draw2DToBack();
	void Draw2DToForward();

	void CreateObject(const int& modelType, const ModelData& modelData = {});
	void DestroyObject(const std::string& name);
	GameObject* GetGameObject(const std::string& name);
	GameObject& GetGameObject2(const std::string& name);
};