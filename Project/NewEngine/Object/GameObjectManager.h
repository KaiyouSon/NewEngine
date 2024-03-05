#pragma once
#include "GameObject.h"

class GameObjectManager
{
private:
	std::vector<std::unique_ptr<GameObject>> mGameObjects;

public:
	void Update();
	void ExecuteCS();
	void AppedToRenderer();
	void Draw();

public:
	// Json関連
	void LoadToJson(nlohmann::json& objectsField);
	nlohmann::json SaveToJson();

public:
	void AddGameObject(const GameObjectType type, const std::string name);
	void DestroyGameObject(const std::string name);

public:
	// ゲッター
	GameObject* GetGameObject(const std::string name);
	std::vector<std::unique_ptr<GameObject>>* GetGameObjects();

};

void AddGameObject(const GameObjectType type, const std::string name);
void DestroyGameObject(const std::string name);
GameObject* GetGameObject(const std::string name);
std::vector<std::unique_ptr<GameObject>>* GetGameObjects();